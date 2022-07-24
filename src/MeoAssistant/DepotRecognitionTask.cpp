#include "DepotRecognitionTask.h"

#include <future>

#include <meojson/json.hpp>

#include "Logger.hpp"
#include "Controller.h"
#include "TaskData.h"
#include "DepotImageAnalyzer.h"
#include "Resource.h"
#include "ProcessTask.h"

bool asst::DepotRecognitionTask::_run()
{
    LogTraceFunction;

    bool ret = swipe_and_analyze();
    callback_analyze_result();

    return ret;
}

bool asst::DepotRecognitionTask::swipe_and_analyze()
{
    LogTraceFunction;
    m_all_items.clear();

    size_t pre_pos = 0ULL;
    while (true) {
        DepotImageAnalyzer analyzer(m_ctrler->get_image());

        auto future = std::async(std::launch::async, [&]() {
            this->swipe();
        });

        // 因为滑动不是完整的一页，有可能上一次识别过的物品，这次仍然在页面中
        // 所以这个 begin pos 不能设置
        //analyzer.set_match_begin_pos(pre_pos);
        if (!analyzer.analyze()) {
            break;
        }
        size_t cur_pos = analyzer.get_match_begin_pos();
        if (cur_pos == pre_pos || cur_pos == DepotImageAnalyzer::NPos) {
            break;
        }
        pre_pos = cur_pos;

        auto cur_result = analyzer.get_result();
        m_all_items.merge(std::move(cur_result));

        future.wait();
    }
    return m_all_items.empty();
}

void asst::DepotRecognitionTask::callback_analyze_result()
{
    LogTraceFunction;

    auto& templ = Resrc.cfg().get_options().depot_export_template;
    json::value info = basic_info_with_what("DepotInfo");
    auto& details = info["details"];

    auto arkplanner_template_opt = json::parse(templ.ark_planner);
    if (arkplanner_template_opt) {
        auto& arkplanner = details["arkplanner"];
        auto& arkplanner_obj = arkplanner["object"];
        arkplanner_obj = arkplanner_template_opt.value();
        auto& arkplanner_data_items = arkplanner_obj["items"];

        for (const auto& [item_id, item_info] : m_all_items) {
            arkplanner_data_items.array_emplace(
                json::object({
                    { "id", item_id },
                    { "have", item_info.quantity },
                    { "name", item_info.item_name }
                    })
            );
        }
        arkplanner["data"] = arkplanner_obj.to_string();
    }
    callback(AsstMsg::SubTaskExtraInfo, info);
}

void asst::DepotRecognitionTask::swipe()
{
    LogTraceFunction;
    static Rect right_rect = Task.get("DepotTaskSlowlySwipeRightRect")->specific_rect;
    static Rect left_rect = Task.get("DepotTaskSlowlySwipeLeftRect")->specific_rect;
    static int duration = Task.get("DepotTaskSlowlySwipeRightRect")->pre_delay;
    static int extra_delay = Task.get("DepotTaskSlowlySwipeRightRect")->rear_delay;

    m_ctrler->swipe(right_rect, left_rect, duration, true, extra_delay, true);
}

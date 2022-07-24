#include "RoguelikeSkillSelectionImageAnalyzer.h"

#include "AsstUtils.hpp"
#include "OcrWithPreprocessImageAnalyzer.h"
#include "MultiMatchImageAnalyzer.h"
#include "TaskData.h"
#include "Logger.hpp"
#include "Resource.h"

bool asst::RoguelikeSkillSelectionImageAnalyzer::analyze()
{
    MultiMatchImageAnalyzer flag_analyzer(m_image);
    flag_analyzer.set_task_info("Roguelike1SkillSelectionFlag");

    if (!flag_analyzer.analyze()) {
        return false;
    }

    const auto& flags = flag_analyzer.get_result();

    if (flags.size() > 13) {
        // https://github.com/MaaAssistantArknights/MaaAssistantArknights/issues/669
        // 不知道为什么会匹配出来一堆结果，得分完全相同，坐标间隔还特别有规律。这种情况直接报错然后重试
        Log.error("Too many flags");
        return false;
    }

    int unknow_index = 0;
    for (const auto& flag : flags) {
        std::string name = name_analyze(flag.rect);
        auto skills = skill_analyze(flag.rect);
        if (skills.empty()) {
            continue;
        }
        if (name.empty()) {
            name = "Unknown" + std::to_string(unknow_index++);
        }
        m_result.emplace(std::move(name), std::move(skills));
    }

    return !m_result.empty();
}

std::string asst::RoguelikeSkillSelectionImageAnalyzer::name_analyze(const Rect& roi)
{
    OcrWithPreprocessImageAnalyzer analyzer;
    auto name_task_ptr = Task.get<OcrTaskInfo>("Roguelike1SkillSelectionName");
    analyzer.set_task_info(name_task_ptr);
    analyzer.set_image(m_image, roi.move(name_task_ptr->roi));
    analyzer.set_required(Resrc.roguelike_recruit().get_oper_order());
    analyzer.set_replace(
        std::dynamic_pointer_cast<OcrTaskInfo>(
            Task.get("CharsNameOcrReplace"))
        ->replace_map);

    if (!analyzer.analyze()) {
        return {};
    }
    return analyzer.get_result().front().text;
}

std::vector<asst::Rect> asst::RoguelikeSkillSelectionImageAnalyzer::skill_analyze(const Rect& roi)
{
    const auto task_ptr = Task.get<MatchTaskInfo>("Roguelike1SkillSelectionRect");
    Rect cor_roi = roi.move(task_ptr->roi);
    cv::Mat image_roi = m_image(utils::make_rect<cv::Rect>(cor_roi));

    std::vector<Rect> result_vec;

    int skill_width = cor_roi.width / MaxNumOfSkills;
    for (int x = 0; x + skill_width < cor_roi.width; x += skill_width) {
        Rect single_skill_rect(x, 0, skill_width, cor_roi.height);
        cv::Mat skill = image_roi(utils::make_rect<cv::Rect>(single_skill_rect));
        cv::Mat skill_gray;
        cv::cvtColor(skill, skill_gray, cv::COLOR_BGR2GRAY);
        cv::Scalar avg = cv::mean(skill_gray);
        Log.trace(__FUNCTION__, "gray avg", avg[0]);
        if (avg[0] < task_ptr->templ_threshold) {
            break;
        }
        Rect result = cor_roi.move(single_skill_rect.move(task_ptr->rect_move));
#ifdef ASST_DEBUG
        cv::rectangle(m_image_draw, utils::make_rect<cv::Rect>(result), cv::Scalar(0, 255, 0), 3);
#endif
        result_vec.emplace_back(result);
    }

    return result_vec;
}

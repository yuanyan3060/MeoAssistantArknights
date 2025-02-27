#include "OcrWithPreprocessImageAnalyzer.h"

#include "AsstUtils.hpp"

bool asst::OcrWithPreprocessImageAnalyzer::analyze()
{
    m_without_det = true;

    cv::Mat img_roi = m_image(utils::make_rect<cv::Rect>(m_roi));
    cv::Mat img_roi_gray;
    cv::cvtColor(img_roi, img_roi_gray, cv::COLOR_BGR2GRAY);
    cv::Mat bin;
    cv::inRange(img_roi_gray, m_threshold_lower, m_threshold_upper, bin);
    cv::Rect bounding_rect = cv::boundingRect(bin);
    bounding_rect.x += m_roi.x;
    bounding_rect.y += m_roi.y;
    auto new_roi = utils::make_rect<Rect>(bounding_rect);

    if (new_roi.empty()) {
        return false;
    }
    // todo: split

    if (m_expansion) {
        new_roi.x -= m_expansion;
        new_roi.y -= m_expansion;
        new_roi.width += 2 * m_expansion;
        new_roi.height += 2 * m_expansion;
    }
    OcrImageAnalyzer::set_roi(new_roi);
#ifdef ASST_DEBUG
    cv::rectangle(m_image_draw, utils::make_rect<cv::Rect>(new_roi), cv::Scalar(0, 0, 255), 1);
#endif // ASST_DEBUG

    return OcrImageAnalyzer::analyze();
}

void asst::OcrWithPreprocessImageAnalyzer::set_threshold(int lower, int upper)
{
    m_threshold_lower = lower;
    m_threshold_upper = upper;
}

void asst::OcrWithPreprocessImageAnalyzer::set_split(bool split)
{
    m_split = split;
}

void asst::OcrWithPreprocessImageAnalyzer::set_expansion(int expansion)
{
    m_expansion = expansion;
}

void asst::OcrWithPreprocessImageAnalyzer::set_task_info(std::shared_ptr<TaskInfo> task_ptr)
{
    OcrImageAnalyzer::set_task_info(task_ptr);
}

void asst::OcrWithPreprocessImageAnalyzer::set_task_info(const std::string& task_name)
{
    OcrImageAnalyzer::set_task_info(task_name);
}

void asst::OcrWithPreprocessImageAnalyzer::set_task_info(OcrTaskInfo task_info) noexcept
{
    m_required = std::move(task_info.text);
    m_full_match = task_info.full_match;
    m_replace = std::move(task_info.replace_map);

    set_roi(task_info.roi);
    m_use_cache = true;
    m_without_det = true;
}

#pragma once

#include "AbstractTask.h"

#include <memory>

namespace asst
{
    class RecruitCalcTask : public AbstractTask
    {
    public:
        using AbstractTask::AbstractTask;
        virtual ~RecruitCalcTask() = default;

        RecruitCalcTask& set_param(std::vector<int> select_level, bool set_time = true, bool skip_robot = true) noexcept;

        bool get_has_special_tag() const noexcept
        {
            return m_has_special_tag;
        }
        bool get_has_refresh() const noexcept
        {
            return m_has_refresh;
        }
        int get_maybe_level() const noexcept
        {
            return m_maybe_level;
        }
        bool get_has_robot_tag() const noexcept
        {
            return m_has_robot_tag;
        }

    protected:
        virtual bool _run() override;

        /* 外部设置参数 */
        std::vector<int> m_select_level;
        bool m_set_time = false;

        /* 内部处理用参数*/
        int m_maybe_level = 0;
        bool m_has_special_tag = false;
        bool m_has_robot_tag = false;
        bool m_has_refresh = false;
        bool m_skip_robot = true;
    };
}

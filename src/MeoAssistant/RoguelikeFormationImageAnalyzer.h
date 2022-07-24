#pragma once
#include "AbstractImageAnalyzer.h"

namespace asst
{
    class RoguelikeFormationImageAnalyzer final : public AbstractImageAnalyzer
    {
    public:
        struct FormationOper
        {
            Rect rect;
            bool selected = false;
            // TODO
        };
    public:
        using AbstractImageAnalyzer::AbstractImageAnalyzer;
        virtual ~RoguelikeFormationImageAnalyzer() = default;

        virtual bool analyze() override;

        const std::vector<FormationOper>& get_result() const noexcept;
    protected:
        // 该分析器不支持外部设置ROI
        virtual void set_roi(const Rect& roi) noexcept override
        {
            AbstractImageAnalyzer::set_roi(roi);
        }

        bool selected_analyze(const Rect& roi);

        std::vector<FormationOper> m_result;
    };
}

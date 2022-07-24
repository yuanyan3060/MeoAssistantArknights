#pragma once
#include "AbstractTaskPlugin.h"

#include <unordered_map>

namespace asst
{
    class RoguelikeSkillSelectionTaskPlugin final : public AbstractTaskPlugin
    {
    public:
        using AbstractTaskPlugin::AbstractTaskPlugin;
        virtual ~RoguelikeSkillSelectionTaskPlugin() = default;

        virtual bool verify(AsstMsg msg, const json::value& details) const override;

    protected:
        virtual bool _run() override;
    };
}

#include "StartGameTaskPlugin.h"

#include "Controller.h"

using namespace asst;

bool StartGameTaskPlugin::_run()
{
    if (m_client_type.empty()) {
        return false;
    }
    return m_ctrler->start_game(m_client_type).has_value();
}

StartGameTaskPlugin& StartGameTaskPlugin::set_client_type(std::string client_type) noexcept
{
    m_client_type = std::move(client_type);
    return *this;
}

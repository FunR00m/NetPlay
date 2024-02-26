#include "Keyboard.hpp"
#include "utils/debug.hpp"

namespace engine
{

Keyboard::Keyboard(std::shared_ptr<Controller> controller)
{
    m_controller = controller;
}

void Keyboard::set_state(Key key, bool state)
{
    m_key_states[key] = state;
    if(m_assigned_controls.contains(key))
    {
        m_controller->set_state(m_assigned_controls[key], state);
    }
}

void Keyboard::press(Key key)
{
    set_state(key, true);
}

void Keyboard::release(Key key)
{
    set_state(key, false);
}

bool Keyboard::get_state(Key key)
{
    if(m_key_states.find(key) == m_key_states.end())
    {
        m_key_states[key] = false;
    }

    return m_key_states[key];
}

void Keyboard::assign_control(Key key, Controller::Control control)
{
    m_assigned_controls[key] = control;
}

void Keyboard::unassign_key(Key key)
{
    m_assigned_controls.erase(key);
}

}

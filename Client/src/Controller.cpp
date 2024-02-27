#include "Controller.hpp"
#include "Fields.hpp"
#include "utils/debug.hpp"

namespace engine
{

Controller::Controller()
{
    m_locked = false;
}

void Controller::set_state(Control control, bool state)
{
    if(!m_locked)
    {
        m_control_states[control] = state;
        m_changed_controls.push_back(control);
    }
}

void Controller::press(Control control)
{
    set_state(control, true);
}

void Controller::release(Control control)
{
    set_state(control, false);
}

bool Controller::get_state(Control control)
{
    if(m_control_states.find(control) == m_control_states.end())
    {
        m_control_states[control] = false;
    }

    return m_control_states[control];
}

PackedData Controller::pack()
{
    // XXX Пока не реализовано из-за отсутствия полного списка клавиш
    // в словаре состояний m_key_states.
    fixme("[Controller::pack] stub");
    return fetch_changes();
}

PackedData Controller::fetch_changes()
{
    PackedData data;
    
    // Добавляем количество изменённых клавиш
    data += IntField(m_changed_controls.size()).pack();

    // Добавляем информацию о каждой изменённой клавише
    for(Control control : m_changed_controls)
    {
        // Номер клавишы
        data += IntField(control).pack();

        // Состояние клавишы. bool автоматически будет переведён в 0
        // или 1.
        data += IntField(m_control_states[control]).pack();
    }

    // Очищаем список измененённых клавиш
    m_changed_controls.clear();

    return data;
}

void Controller::lock()
{
    m_locked = true;
}

void Controller::unlock()
{
    m_locked = false;
}

}

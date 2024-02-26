#include "Controller.hpp"
#include "Fields.hpp"
#include "utils/debug.hpp"

namespace engine
{

Controller::Controller()
{
    
}

bool Controller::get_state(Control control)
{
    if(m_control_states.find(control) == m_control_states.end())
    {
        m_control_states[control] = false;
    }

    return m_control_states[control];
}

void Controller::apply_changes(PackedData data)
{
    // Распаковываем количество изменённых элементов
    IntField size;
    size.unpack(data.take());

    // Обрабатываем каждый элемент
    for(int i = 0; i < size; i++)
    {
        IntField control, state;
        control.unpack(data.take());
        state.unpack(data.take());

        int control_int = control;
        m_control_states[control_int] = state;
    }
}

}

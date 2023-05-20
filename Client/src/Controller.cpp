#include "Controller.hpp"
#include "Fields.hpp"
#include "utils/debug.hpp"

namespace engine
{

Controller::Controller()
{
    
}

void Controller::set_state(Key key, bool state)
{
    m_key_states[key] = state;
    m_changed_keys.push_back(key);
}

void Controller::press(Key key)
{
    set_state(key, true);
}

void Controller::release(Key key)
{
    set_state(key, false);
}

bool Controller::get_state(Key key)
{
    if(m_key_states.find(key) == m_key_states.end())
    {
        m_key_states[key] = false;
    }

    return m_key_states[key];
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
    data += IntField(m_changed_keys.size()).pack();

    // Добавляем информацию о каждой изменённой клавише
    for(Key key : m_changed_keys)
    {
        // Номер клавишы
        data += IntField(key).pack();

        // Состояние клавишы. bool автоматически будет переведён в 0
        // или 1.
        data += IntField(m_key_states[key]).pack();
    }

    // Очищаем список измененённых клавиш
    m_changed_keys.clear();

    return data;
}

}

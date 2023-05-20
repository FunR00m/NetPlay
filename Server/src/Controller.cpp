#include "Controller.hpp"
#include "Fields.hpp"
#include "utils/debug.hpp"

namespace engine
{

Controller::Controller()
{
    
}

bool Controller::get_state(Key key)
{
    if(m_key_states.find(key) == m_key_states.end())
    {
        m_key_states[key] = false;
    }

    return m_key_states[key];
}

void Controller::apply_changes(PackedData data)
{
    // Распаковываем количество изменённых клавиш
    IntField size;
    size.unpack(data.take());

    // Обрабатываем каждую клавишу
    for(int i = 0; i < size; i++)
    {
        IntField key, state;
        key.unpack(data.take());
        state.unpack(data.take());

        int key_int = key;
        m_key_states[key_int] = state;
    }
}

}

#include "Systems/MoveSystem.hpp"

namespace engine
{

void MoveSystem::start(GameManager *game_manager)
{
    m_game_manager = game_manager;
}

void MoveSystem::tick()
{
    for(auto obj : m_game_manager->get_objects())
    {
        auto transform = obj->get_component<Transform>();
        auto sprite = obj->get_component<Sprite>();
        if(!transform)
        {
            continue;
        }

        transform->pos += transform->motion;
        transform->move_event.invoke(transform->pos);
        if(sprite)
        {
            sprite->pos = transform->pos;
        }
    }
}

}

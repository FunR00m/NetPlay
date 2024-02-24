#include "Systems/MoveSystem.hpp"
#include "Components/Collider.hpp"

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
        auto collider = obj->get_component<Collider>();
        if(!transform)
        {
            continue;
        }

        transform->pos += transform->motion;
        transform->move_event.invoke(transform->pos);

        if(collider)
        {
            // debug("Collider. X: " + std::to_string(transform->pos.x) + " Y: " + std::to_string(transform->pos.y));
            collider->pos = transform->pos;
            for(auto obj_2 : m_game_manager->get_objects())
            {
                if(obj_2 == obj)
                {
                    continue;
                }

                auto collider_2 = obj_2->get_component<Collider>();
                if(collider->collision(collider_2))
                {
                    transform->pos -= transform->motion;
                    collider->pos = transform->pos;
                }
            }
        }
    }
}

}

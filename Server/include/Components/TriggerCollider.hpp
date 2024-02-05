#ifndef TRIGGER_COLLIDER_HPP
#define TRIGGER_COLLIDER_HPP

#include "Components/Collider.hpp"

namespace engine
{

/// @brief На данный момент ничем не отличается от родительского
/// класса, но система обработки столкновений не замечает объекты
/// данного типа.
class TriggerCollider : public Collider
{
public:
    std::string get_name() override;

};

}

#endif

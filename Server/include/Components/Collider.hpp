#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "IComponent.hpp"
#include "Fields.hpp"
#include "Event.hpp"

namespace engine
{

/// @brief Описывает модель столкновений
class Collider : public IComponent
{
public:
    Vec2Field rect;
    Vec2Field pos;
    
    Collider();
    
    Collider(int x, int y);

    PackedData pack() override;
    
    PackedData fetch_changes() override;

    void unpack(PackedData data) override;

    void apply_changes(PackedData data) override;

    virtual std::string get_name() override;

    /// @returns ```true```, если есть пересечение с другим
    /// объектом типа collider.
    /// @param other Указатель на другой объект типа collider
    bool collision(std::shared_ptr<Collider> other);

};


}

#endif

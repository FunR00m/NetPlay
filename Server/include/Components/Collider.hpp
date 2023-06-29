#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "IComponent.hpp"
#include "Fields.hpp"
#include "Event.hpp"

namespace engine
{

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

    std::string get_name() override;

    bool collision(std::shared_ptr<Collider> other);

};


}

#endif

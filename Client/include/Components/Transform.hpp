#ifndef Transform_hpp
#define Transform_hpp

#include "IComponent.hpp"
#include "Fields.hpp"
#include "Event.hpp"

namespace engine
{

class Transform : public IComponent
{
public:
    Vec2Field pos;
    Vec2Field motion;
    
    Event<Vec2Field> move_event;
    
    Transform();

    PackedData pack() override;
    
    PackedData fetch_changes() override;

    void unpack(PackedData data) override;

    void apply_changes(PackedData data) override;
};


}

#endif

#ifndef Sprite_hpp
#define Sprite_hpp

#include "IComponent.hpp"
#include "Fields.hpp"
#include "Event.hpp"

namespace engine
{

class Sprite : public IComponent
{
public:
    StringField name;
    Vec2Field size;
    
    Sprite();

    PackedData pack() override;
    
    PackedData fetch_changes() override;

    void unpack(PackedData data) override;

    void apply_changes(PackedData data) override;

    std::string get_name() override;
};


}

#endif

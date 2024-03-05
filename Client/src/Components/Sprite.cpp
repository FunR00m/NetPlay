#include "Components/Sprite.hpp"

namespace engine
{

Sprite::Sprite()
{
    name = std::string("sprites/void.png");
    size = { 100, 100 };
    visible = true;
}

PackedData Sprite::pack()
{
    PackedData data;
    data += name.pack();
    data += size.pack();
    data += z_layer.pack();
    data += visible.pack();
    return data;
}

PackedData Sprite::fetch_changes()
{
    PackedData data;
    data += name.fetch_changes();
    data += size.fetch_changes();
    data += z_layer.fetch_changes();
    data += visible.fetch_changes();
    return data;
}

void Sprite::unpack(PackedData data)
{
    name.unpack(data.take());
    size.unpack(data.take());
    z_layer.unpack(data.take());
    visible.unpack(data.take());
}

void Sprite::apply_changes(PackedData data)
{
    name.apply_changes(data.take());
    size.apply_changes(data.take());
    z_layer.apply_changes(data.take());
    visible.apply_changes(data.take());
}

}

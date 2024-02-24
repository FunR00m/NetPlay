#include "Components/Sprite.hpp"

namespace engine
{

Sprite::Sprite()
{
    name = std::string("Empty");
    size = { 100, 100 };
}

PackedData Sprite::pack()
{
    PackedData data;
    data += name.pack();
    data += size.pack();
    return data;
}

PackedData Sprite::fetch_changes()
{
    PackedData data;
    data += name.fetch_changes();
    data += size.fetch_changes();
    return data;
}

void Sprite::unpack(PackedData data)
{
    name.unpack(data.take());
    size.unpack(data.take());
}

void Sprite::apply_changes(PackedData data)
{
    name.apply_changes(data.take());
    size.apply_changes(data.take());
}

std::string Sprite::get_name()
{
    return "Sprite";
}

}

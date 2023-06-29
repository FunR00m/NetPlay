#include "Components/Collider.hpp"

namespace engine
{

Collider::Collider()
{
    rect = { 0, 0 };
}

Collider::Collider(int x, int y)
{
    rect = { x, y };
}

PackedData Collider::pack()
{
    return PackedData();
}

PackedData Collider::fetch_changes()
{
    return PackedData();
}

void Collider::unpack(PackedData data)
{
    return;
}

void Collider::apply_changes(PackedData data)
{
    return;
}

std::string Collider::get_name()
{
    return "Collider";
}

bool Collider::collision(std::shared_ptr<Collider> other)
{
    if(other == nullptr)
    {
        return false;
    } else if(pos.x > other->pos.x + other->rect.x)
    {
        return false;
    } else if(pos.x + rect.x < other->pos.x)
    {
        return false;
    } else if(pos.y > other->pos.y + other->rect.y)
    {
        return false;
    } else if(pos.y + rect.y < other->pos.y)
    {
        return false;
    } else {
        return true;
    }
}

}
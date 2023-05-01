#include "Components/Transform.hpp"

namespace engine
{

Transform::Transform()
{
    pos = { 0, 0 };
    motion = { 0, 0 };
}

PackedData Transform::pack()
{
    PackedData data;
    data += pos.pack();
    data += motion.pack();
    return data;
}

PackedData Transform::fetch_changes()
{
    PackedData data;
    data += pos.fetch_changes();
    data += motion.fetch_changes();
    return data;
}

void Transform::unpack(PackedData data)
{
    pos.unpack(data.take());
    motion.unpack(data.take());
}

void Transform::apply_changes(PackedData data)
{
    pos.apply_changes(data.take());
    motion.apply_changes(data.take());
}

}

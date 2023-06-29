#ifndef NULLHOMP_HPP
#define NULLHOMP_HPP

#include "IComponent.hpp"

namespace engine
{

class NullComp : public IComponent
{
public:
    PackedData pack() { return PackedData(); };

    PackedData fetch_changes() { return PackedData(); };

    void unpack(PackedData data) { return; };

    void apply_changes(PackedData data) { return; };
};

}

#endif
//
//  IComponent.hpp
//  Server
//
//  Created by Fedor Fedor on 03.04.2023.
//

#ifndef Component_hpp
#define Component_hpp

#include <unordered_map>
#include <vector>
#include <memory>

#include "IField.hpp"
#include "PackedData.hpp"

namespace engine
{

class IComponent
{
public:
    virtual PackedData pack() = 0;
    virtual PackedData fetch_changes() = 0;
};

}

#endif /* Component_hpp */

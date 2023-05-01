//
//  ISystem.hpp
//  NetPlay
//
//  Created by Fedor Fedor on 07.04.2023.
//

#ifndef ISystem_hpp
#define ISystem_hpp

namespace engine
{

class GameManager;

class ISystem
{
public:
    virtual void start(GameManager *game_manager) = 0;
    virtual void tick() = 0;
};

}

#endif /* ISystem_hpp */

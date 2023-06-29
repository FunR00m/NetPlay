//
//  ISystem.hpp
//  NetPlay
//
//  Created by Fedor Fedor on 07.04.2023.
//

#ifndef ISYSTEM_HPP
#define ISYSTEM_HPP

namespace engine
{

class GameManager;

class ISystem
{
public:
    virtual void start(GameManager *game_manager) = 0;
    virtual void tick() = 0;
    virtual void stop() = 0;
};

}

#endif /* ISystem_hpp */

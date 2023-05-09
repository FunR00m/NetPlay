#ifndef MoveSystem_hpp
#define MoveSystem_hpp

#include "GameManager.hpp"
#include "Components/Transform.hpp"
#include "Components/Sprite.hpp"

namespace engine
{

class MoveSystem : public ISystem
{
public:
    void start(GameManager *game_manager) override;
    
    void tick() override;

private:
    GameManager *m_game_manager;
};

}

#endif

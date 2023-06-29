#ifndef RenderSystem_hpp
#define RenderSystem_hpp

#include <unordered_map>
#include <memory>
#include <string>

#include "ISystem.hpp"
#include "Components/Sprite.hpp"
#include "sys/TextureSDL.hpp"
#include "sys/RendererSDL.hpp"

namespace engine
{

class RenderSystem : public ISystem
{
public:
    using ActiveTexture = sys::TextureSDL;
    using ActiveRenderer = sys::RendererSDL;

    void start(GameManager *game_manager) override;

    void tick() override;

    void stop() override;

    void load_texture(std::string name);

private:
    GameManager *m_game;
    std::unordered_map<std::string, std::shared_ptr<ActiveTexture>> m_textures;
    ActiveRenderer m_renderer;

};

}

#endif

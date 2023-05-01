#include "Systems/RenderSystem.hpp"
#include "GameManager.hpp"
#include "IRenderer.hpp"
#include "utils/debug.hpp"

namespace engine
{

void RenderSystem::start(GameManager *game_manager)
{
    m_game = game_manager;
    m_renderer.setup();
}

void RenderSystem::tick()
{
    for(auto object : m_game->get_objects())
    {
        std::shared_ptr<Sprite> sprite = object->get_component<Sprite>();
        if(!sprite)
        {
            continue;
        }

        if(m_textures.find(sprite->name) == m_textures.end())
        {
            warning(std::string("[RenderSystem::tick] Texture '") + 
            sprite->name.s() + std::string("' has not been loaded. Loading it."));
            load_texture(sprite->name);
        }
        sprite->texture = m_textures[sprite->name];

        m_renderer.render_sprite(sprite);
    }
    m_renderer.refresh();
}

void RenderSystem::load_texture(std::string name)
{
    std::shared_ptr<ActiveTexture> texture = std::make_shared<ActiveTexture>();
    texture->load(name);
    m_textures[name] = texture;
}

}

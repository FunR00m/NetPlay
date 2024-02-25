#include "Systems/RenderSystem.hpp"
#include "Components/Transform.hpp"
#include "GameManager.hpp"
#include "IRenderer.hpp"
#include "utils/debug.hpp"

namespace engine
{

void RenderSystem::start(GameManager *game_manager)
{
    m_game = game_manager;
    m_renderer.setup();
    m_renderer.set_window_title("Engine Test");
}

void RenderSystem::tick()
{
    // Объявляем массив спрайтов
    std::vector< std::shared_ptr<Sprite> > render_queue;

    // Ищем объекты со спрайтами
    for(auto object : m_game->get_objects())
    {
        std::shared_ptr<Sprite> sprite = object->get_component<Sprite>();
        if(sprite)
        {
            // Загружаем текстуру, если она ещё не загружена
            if(m_textures.find(sprite->name) == m_textures.end())
            {
                // warning(std::string("[RenderSystem::tick] Texture '") + 
                // sprite->name.s() + std::string("' has not been loaded. Loading it."));
                load_texture(sprite->name);
            }

            // Устанавливаем координаты спрайта
            sprite->pos = object->get_absolute_transform().pos;
            
            // Устанавливаем текстуру спрайта
            sprite->texture = m_textures[sprite->name];

            // Добавляем спрайт в очередь, если он видим
            if(sprite->visible)
            {
                render_queue.push_back(sprite);
            }
        }
    }

    // Сортируем очередь спрайтов по параметру z_layer
    std::sort(render_queue.begin(), render_queue.end(),
        [](std::shared_ptr<Sprite> sprite_1, std::shared_ptr<Sprite> sprite_2)
        {
            return sprite_1->z_layer < sprite_2->z_layer;
        }
    );
    
    // Отрисовываем спрайты
    for(std::shared_ptr<Sprite> sprite : render_queue)
    {
        m_renderer.render_sprite(sprite);
    }

    m_renderer.refresh();
    m_renderer.handle_events(m_game->get_controller());

    if(m_renderer.is_quit())
    {
        m_game->stop();
    }
}

void RenderSystem::stop()
{
    m_renderer.stop();
}

void RenderSystem::load_texture(std::string name)
{
    std::shared_ptr<ActiveTexture> texture = std::make_shared<ActiveTexture>();
    texture->load(name);
    m_textures[name] = texture;
}

}

#include "Systems/RenderSystem.hpp"
#include "Components/Transform.hpp"
#include "GameManager.hpp"
#include "IRenderer.hpp"
#include "Components/AnimatedSprite.hpp"
#include "utils/debug.hpp"

namespace engine
{

void RenderSystem::start(GameManager *game_manager)
{
    m_game = game_manager;
    m_renderer.setup();
    m_renderer.set_window_title("NetPlay");
}

void RenderSystem::tick()
{
    // Объявляем массив видимых спрайтов
    std::vector< std::shared_ptr<Sprite> > render_queue;

    // Ищем спрайты и обновляем их
    for(auto object : m_game->get_objects())
    {
        auto sprite = object->get_component<Sprite>();
        auto animated_sprite = object->get_component<AnimatedSprite>();
        
        // Проверяем, есть ли у объекта спрайт
        if(!(sprite || animated_sprite))
        {
            continue;
        } else if(!sprite)
        {
            // Если обычного спрайта нет, то используем анимированный
            sprite = animated_sprite;
        }

        // Обновляем кадр спрайта, если он анимированный
        if(animated_sprite)
        {
            animated_sprite->update(m_game->get_client_time());
        }

        // Загружаем текстуру, если она ещё не загружена
        if(!m_textures.contains(sprite->name))
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
    m_renderer.handle_events(m_game->get_keyboard());

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

void RenderSystem::set_window_title(std::string title)
{
    m_renderer.set_window_title(title);
}

void RenderSystem::set_window_size(int width, int height)
{
    m_renderer.set_window_size(width, height);
}

void RenderSystem::set_scale(float scale)
{
    m_renderer.set_scale(scale);
}

void RenderSystem::enable_fullscreen()
{
    m_renderer.enable_fullscreen();
}

void RenderSystem::disable_fullscreen()
{
    m_renderer.disable_fullscreen();
}

}

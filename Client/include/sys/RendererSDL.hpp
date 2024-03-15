//
//  RendererSDL.hpp
//  Point-n-Click
//
//  Created by Fedor Fedor on 08.02.2023.
//

#ifndef RendererSDL_hpp
#define RendererSDL_hpp

#include <memory>
#include <SDL2/SDL.h>

#include "IRenderer.hpp"
#include "TextureSDL.hpp"

namespace engine
{
namespace sys
{

class RendererSDL : public IRenderer
{
public:
    RendererSDL();

    /// Sets the renderer up, for example creates the window.
    void setup() override;
    
    /// Handles system events such as key presses or mouse clicks.
    void handle_events(std::shared_ptr<Keyboard> keyboard) override;
    
    /// Renders the given sprite.
    virtual void render_sprite(std::shared_ptr<Sprite> sprite) override;
    
    /// Displays everething and cleans the screen.
    void refresh() override;
    
    /// Stops the renderer, for example destroys the window.
    void stop() override;
    
    /// @brief Устанавливает заголовок окна
    /// @param title Новый заголовок
    void set_window_title(std::string title) override;

    /// @return ```true```, если окно было закрыто, иначе ```false```
    bool is_quit() override;

    /// @brief Устанавливает размер окна
    /// @param width Ширина в пикселах
    /// @param height Высота в пикселах
    void set_window_size(int width, int height);

    /// @brief Устанавливает масштаб окна
    /// @param scale Коэффициент масштабирования
    void set_scale(float scale);

    /// @brief Вкючает полноэкранный режим
    void enable_fullscreen();

    /// @brief Выключает полноэкранный режим
    void disable_fullscreen();
    
private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    std::string m_window_title;
    
    bool m_quit;
    bool m_running;
    float m_scale;
    
    /// Отношение установленной ширины к установленной высоте окна
    // double m_established_ratio;

    /// Установленные размеры окна
    int m_established_width;
    int m_established_height;

    bool m_fullscreen;
};

}
}

#endif /* RendererSDL_hpp */

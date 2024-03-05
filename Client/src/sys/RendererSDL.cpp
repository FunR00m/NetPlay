//
//  RendererSDL.cpp
//  Point-n-Click
//
//  Created by Fedor Fedor on 08.02.2023.
//

#include "sys/RendererSDL.hpp"
#include "utils/debug.hpp"

namespace engine
{
namespace sys
{

void RendererSDL::setup()
{
    m_quit = false;
    m_scale = 1.0f;

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    m_window = SDL_CreateWindow(
            "0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            800 * m_scale, 600 * m_scale,
             0);
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    set_window_size(800, 600);
    set_window_title(m_window_title);
}

void RendererSDL::handle_events(std::shared_ptr<Keyboard> keyboard)
{
    SDL_Event sdl_event;
    while(SDL_PollEvent(&sdl_event))
    {
        switch(sdl_event.type)
        {
            case SDL_KEYDOWN:
                keyboard->press(SDL_GetKeyFromScancode(sdl_event.key.keysym.scancode));
                break;
            case SDL_KEYUP:
                keyboard->release(SDL_GetKeyFromScancode(sdl_event.key.keysym.scancode));
                break;
            case SDL_QUIT:
                // fixme("core::sys::RendererSDL::poll_events() SDL_QUIT event has not been implemented yet.");
                m_quit = true;
                break;
        }
    }
}

bool RendererSDL::is_quit()
{
    return m_quit;
}

void RendererSDL::render_sprite(std::shared_ptr<Sprite> sprite)
{
    std::shared_ptr<ITexture> texture = sprite->texture;
    Vec2Field pos = sprite->pos;
    Vec2Field size = sprite->size;
    
    if(!texture->is_loaded())
    {
        return;
    }
    
    std::shared_ptr<TextureSDL> native_texture = std::dynamic_pointer_cast<TextureSDL>(texture);
    if(!native_texture)
    {
        error("Failed to render texture: dynamic_pointer_cast returned NULL");
        return;
    }
    
    SDL_Texture *sdl_texture = SDL_CreateTextureFromSurface(m_renderer, native_texture->get_data());

    // Определяем смещение, чтобы центр находился в центре окна. Это
    // необходимо при включении полноэкранного режима, когда
    // соотношение сторон экрана не совпадает с используемым в проекте.
    int offset_x, offset_y;
    int window_width, window_height;
    SDL_GetWindowSize(m_window, &window_width, &window_height);
    double window_ratio = (
                        static_cast<double>(window_width) /
                        static_cast<double>(window_height)
    );
    if(window_ratio > m_established_ratio)
    {
        offset_x = (window_width - m_established_ratio * window_height) / 2;
        offset_y = 0;
    } else {
        offset_x = 0;
        offset_y = (window_height - window_width / m_established_ratio) / 2;
    }



    SDL_Rect dstrect = {
        (int)(pos.x * m_scale + offset_x),
        (int)(pos.y * m_scale + offset_y),
        (int)(size.x * m_scale),
        (int)(size.y * m_scale)
    };

    SDL_RenderCopy(m_renderer, sdl_texture, NULL, &dstrect);
    SDL_DestroyTexture(sdl_texture);
}

void RendererSDL::refresh()
{
    SDL_RenderPresent(m_renderer);
    SDL_RenderClear(m_renderer);
}

void RendererSDL::stop()
{
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
    IMG_Quit();
}

void RendererSDL::set_window_title(std::string title)
{
    SDL_version version;
    SDL_GetVersion(&version);
    SDL_SetWindowTitle(m_window, (title + " (SDL " + std::to_string(version.major) + "." + std::to_string(version.minor) + "." + std::to_string(version.patch) + ")").c_str());
    m_window_title = title;
}

void RendererSDL::set_window_size(int width, int height)
{
    SDL_SetWindowSize(m_window, width * m_scale, height * m_scale);
    m_established_ratio = static_cast<double>(width) / static_cast<double>(height);
}

void RendererSDL::set_scale(float scale)
{
    m_scale = scale;
}

void RendererSDL::enable_fullscreen()
{
    m_fullscreen = true;
    SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
}

void RendererSDL::disable_fullscreen()
{
    m_fullscreen = false;
    SDL_SetWindowFullscreen(m_window, 0);
}

}
}

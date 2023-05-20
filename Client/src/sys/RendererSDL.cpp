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
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    window_ = SDL_CreateWindow("0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    set_window_title(window_title_);
    renderer_ = SDL_CreateRenderer(window_, -1, 0);
}

void RendererSDL::handle_events(std::shared_ptr<Controller> controller)
{
    SDL_Event sdl_event;
    while(SDL_PollEvent(&sdl_event))
    {
        switch(sdl_event.type)
        {
            case SDL_KEYDOWN:
                controller->press(SDL_GetKeyFromScancode(sdl_event.key.keysym.scancode));
                break;
            case SDL_KEYUP:
                controller->release(SDL_GetKeyFromScancode(sdl_event.key.keysym.scancode));
                break;
            case SDL_QUIT:
                fixme("core::sys::RendererSDL::poll_events() SDL_QUIT event has not been implemented yet.");
                break;
        }
    }
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
    
    SDL_Texture *sdl_texture = SDL_CreateTextureFromSurface(renderer_, native_texture->get_data());
    // SDL_SetTextureAlphaMod(sdl_texture, opacity);
    SDL_Rect dstrect = { pos.x, pos.y, size.x, size.y };
    SDL_RenderCopy(renderer_, sdl_texture, NULL, &dstrect);
    SDL_DestroyTexture(sdl_texture);
}

void RendererSDL::refresh()
{
    SDL_RenderPresent(renderer_);
    SDL_RenderClear(renderer_);
}

void RendererSDL::stop()
{
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_Quit();
    IMG_Quit();
}

void RendererSDL::set_window_title(std::string title)
{
    SDL_version version;
    SDL_GetVersion(&version);
    SDL_SetWindowTitle(window_, (title + " (SDL " + std::to_string(version.major) + "." + std::to_string(version.minor) + "." + std::to_string(version.patch) + ")").c_str());
    window_title_ = title;
}

}
}

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

// std::vector<Event> RendererSDL::poll_events()
// {
//     std::vector<Event> events;
//     SDL_Event sdl_event;
//     while(SDL_PollEvent(&sdl_event))
//     {
//         Event event;
//         switch(sdl_event.type)
//         {
//             case SDL_MOUSEBUTTONDOWN:
//                 event.type = Event::MouseDown;
//                 event.pos= { sdl_event.motion.x, sdl_event.motion.y };
//                 break;
//             case SDL_MOUSEBUTTONUP:
//                 event.type = Event::MouseUp;
//                 event.pos= { sdl_event.motion.x, sdl_event.motion.y };
//                 break;
//             case SDL_MOUSEMOTION:
//                 event.type = Event::MouseMove;
//                 event.pos = { sdl_event.motion.x, sdl_event.motion.y };
//                 break;
//             case SDL_KEYDOWN:
//                 fixme("core::sys::RendererSDL::poll_events() SDL_KEYDOWN event has not been implemented yet.");
//                 break;
//             case SDL_KEYUP:
//                 fixme("core::sys::RendererSDL::poll_events() SDL_KEYUP event has not been implemented yet.");
//                 break;
//             case SDL_MOUSEWHEEL:
//                 fixme("core::sys::RendererSDL::poll_events() SDL_MOUSEWHEEL event has not been implemented yet.");
//                 break;
//             case SDL_QUIT:
//                 event.type = Event::GameQuit;
//                 break;
//         }
//         events.push_back(event);
//     }
//     return events;
// }

void RendererSDL::invoke_events()
{
    fixme("[sys::RendererSDL::invoke_events] stub");
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
    
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            debug("Stopping!");
            stop();
            break;
        }
    }
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

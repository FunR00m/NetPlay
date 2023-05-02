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

#include "../IRenderer.hpp"
#include "TextureSDL.hpp"

namespace engine
{
namespace sys
{

class RendererSDL : public IRenderer
{
public:
    /// Sets the renderer up, for example creates the window.
    void setup() override;
    
    /// Returns system events such as key presses or mouse clicks.
    void invoke_events() override;
    
    /// Renders the given sprite.
    virtual void render_sprite(std::shared_ptr<Sprite> sprite) override;
    
    /// Displays everething and cleans the screen.
    void refresh() override;
    
    /// Stops the renderer, for example destroys the window.
    void stop() override;
    
    /// Sets the window title
    /// - Parameter title: The new title
    void set_window_title(std::string title) override;
    
private:
    SDL_Window *window_;
    SDL_Renderer *renderer_;
    std::string window_title_;
};

}
}

#endif /* RendererSDL_hpp */
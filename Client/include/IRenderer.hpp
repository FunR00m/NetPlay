#ifndef IRenderer_hpp
#define IRenderer_hpp

#include <string>
#include <memory>

#include "Components/Sprite.hpp"
#include "Controller.hpp"

namespace engine
{

class IRenderer
{
public:
    /// Sets the renderer up, for example creates the window.
    virtual void setup() = 0;
    
    virtual void handle_events(std::shared_ptr<Controller> controller) = 0;

    /// Renders the given sprite.
    virtual void render_sprite(std::shared_ptr<Sprite> sprite) = 0;
    
    /// Displays everething and cleans the screen.
    virtual void refresh() = 0;
    
    /// Stops the renderer, for example destroys the window.
    virtual void stop() = 0;
    
    /// Sets the window title
    /// - Parameter title: The new title
    virtual void set_window_title(std::string title) = 0;
};

}

#endif

//
//  TextureSDL.hpp
//  Point-n-Click
//
//  Created by Fedor Fedor on 07.02.2023.
//

#ifndef TextureSDL_hpp
#define TextureSDL_hpp

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "ITexture.hpp"

namespace engine
{
namespace sys
{

class TextureSDL : public ITexture
{
public:
    TextureSDL();
    TextureSDL(std::string filename);
    
    /// Returns a pointer to an actual `SDL_Surface`.
    SDL_Surface* get_data();
    
    /// Tells whether the texture is loaded. If it is, it can be rendered.
    bool is_loaded() override;
    
    /// Loads the texture data from file into memory.
    void load() override;
    
    /// Loads the texture data from file into memory.
    /// - Parameter filename: The name of the texture file.
    void load(std::string filename) override;
    
    /// Deletes the texture data from memory.
    void free() override;
    
private:
    SDL_Surface *data_;
    bool loaded_;
    std::string filename_;
};

}
}

#endif /* TextureSDL_hpp */

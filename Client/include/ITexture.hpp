//
//  ITexture.hpp
//  Point-n-Click
//
//  Created by Fedor Fedor on 03.02.2023.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <string>
#include <memory>

namespace engine
{

/// ITexture is an interface class that can store textures for different graphics libraries.
class ITexture
{
public:
    
    /// Tells whether the texture is loaded. If it is, it can be rendered.
    virtual bool is_loaded() = 0;
    
    /// Loads the texture data from file into memory.
    virtual void load() = 0;
    
    /// Loads the texture data from file into memory.
    /// - Parameter filename: The name of the texture file.
    virtual void load(std::string filename) = 0;
    
    /// Deletes the texture data from memory.
    virtual void free() = 0;
};

}

#endif /* Texture_hpp */

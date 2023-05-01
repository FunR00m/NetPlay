//
//  TextureSDL.cpp
//  Point-n-Click
//
//  Created by Fedor Fedor on 07.02.2023.
//

#include "sys/TextureSDL.hpp"
#include "utils/debug.hpp"

namespace engine
{
namespace sys
{

TextureSDL::TextureSDL()
{
    data_ = nullptr;
    loaded_ = false;
}

TextureSDL::TextureSDL(std::string filename)
{
    filename_ = filename;
    data_ = nullptr;
    loaded_ = false;
}

bool TextureSDL::is_loaded()
{
    return loaded_;
}

SDL_Surface* TextureSDL::get_data()
{
    return data_;
}

void TextureSDL::load()
{
    load(filename_);
}

void TextureSDL::load(std::string filename)
{
    data_ = IMG_Load(filename.c_str());
    debug("Loading texture \"" + filename + "\"");
    loaded_  = true;
}

void TextureSDL::free()
{
    SDL_FreeSurface(data_);
}

}
}

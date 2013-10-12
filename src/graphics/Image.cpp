/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#include "nomlib/graphics/Image.hpp"

namespace nom {

Image::Image ( void ) : image_ ( nullptr, priv::FreeSurface )
{
NOM_LOG_TRACE ( NOM );

  if ( IMG_Init ( IMG_INIT_PNG ) != IMG_INIT_PNG )
  {
NOM_LOG_ERR ( NOM, IMG_GetError() );
  }

  atexit ( IMG_Quit );
}

Image::~Image ( void )
{
NOM_LOG_TRACE ( NOM );
}

Image::Image ( uint32 flags ) : image_ ( nullptr, priv::FreeSurface )
{
NOM_LOG_TRACE ( NOM );

  if ( IMG_Init ( flags ) != flags )
  {
NOM_LOG_ERR ( NOM, IMG_GetError() );
  }

  atexit ( IMG_Quit );
}

Image::Image ( const Image& other )  : image_ { other.image(), priv::FreeSurface }
{
NOM_LOG_TRACE ( NOM );
}

Image& Image::operator = ( const Image& other )
{
  this->image_ = other.image_;

  return *this;
}

SDL_Surface* Image::image ( void ) const
{
  return this->image_.get();
}

bool Image::valid ( void ) const
{
  if ( this->image() != nullptr )
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool Image::load ( const std::string& filename )
{
  this->image_.reset ( IMG_Load ( filename.c_str() ), priv::FreeSurface );

  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not load filename at: " + filename );
NOM_LOG_ERR ( NOM, IMG_GetError() );
    return false;
  }

  return true;
}

bool Image::load_bmp ( const std::string& filename )
{
  this->image_.reset ( SDL_LoadBMP ( filename.c_str() ), priv::FreeSurface );

  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not load filename at: " + filename );
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

bool Image::save ( const std::string& filename, SDL_Surface* video_buffer )
{
  if ( SDL_SaveBMP ( video_buffer, filename.c_str() ) != 0 )
  {
NOM_LOG_ERR ( NOM, "Could not save filename at: " + filename );
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

const Point2i Image::size ( void ) const
{
  SDL_Surface* buffer = this->image();
  Point2i image_pos ( buffer->w, buffer->h );

  priv::FreeSurface ( buffer );
  return image_pos;
}

bool Image::set_colorkey ( const Color& key, uint32 flags )
{
  SDL_Surface* buffer = this->image();
  uint32 transparent_color = RGBA::asInt32 ( buffer->format, key );

  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not set color key: invalid image buffer." );
    priv::FreeSurface ( buffer );
    return false;
  }

  if ( SDL_SetColorKey ( buffer, SDL_TRUE ^ flags, transparent_color ) != 0 )
  {
NOM_LOG_ERR ( NOM, std::string ( SDL_GetError() ) );
    priv::FreeSurface ( buffer );
    return false;
  }

  priv::FreeSurface ( buffer );
  return true;
}


} // namespace nom

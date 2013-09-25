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

Image::Image ( int32 flags )  : image_buffer ( nullptr, nom::priv::Canvas_FreeSurface )
{
NOM_LOG_TRACE ( NOM );

  if ( IMG_Init ( flags ) != flags )
  {
NOM_LOG_ERR ( NOM, IMG_GetError() );
  }

  atexit ( IMG_Quit );
}

Image::Image ( const Image& other )  : image_buffer { other.image_buffer.get(), nom::priv::Canvas_FreeSurface }
{
NOM_LOG_TRACE ( NOM );
}

Image::~Image ( void )
{
NOM_LOG_TRACE ( NOM );

  this->image_buffer.reset(); // ...better safe than sorry!
}

bool Image::valid ( void ) const
{
  if ( this->image_buffer.get() != nullptr )
    return true;
  else
    return false;
}

std::shared_ptr<Surface> Image::load ( const std::string& filename )
{
  this->image_buffer.reset ( IMG_Load ( filename.c_str() ), nom::priv::Canvas_FreeSurface );

  if ( ! this->valid() )
  {
NOM_LOG_ERR ( NOM, IMG_GetError() );
    return nullptr;
  }

  return this->image_buffer;
}

std::shared_ptr<Surface> Image::loadBMP ( const std::string& filename )
{
  this->image_buffer = std::shared_ptr<Surface> ( SDL_LoadBMP ( filename.c_str() ), nom::priv::Canvas_FreeSurface );

  if ( ! this->valid() )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return nullptr;
  }

  return this->image_buffer;
}

bool Image::save ( const std::string& filename, Surface* video_buffer )
{
  if ( SDL_SaveBMP ( video_buffer, filename.c_str() ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

const Coords Image::getSize ( void ) const
{
  SDL_Surface *buffer = this->image_buffer.get();

  return Coords ( 0, 0, buffer->w, buffer->h );
}

Image& Image::operator = ( const Image& other )
{
  this->image_buffer = other.image_buffer;

  return *this;
}


} // namespace nom

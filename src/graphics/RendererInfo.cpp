/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include "nomlib/graphics/RendererInfo.hpp"

// Private headers
#include <SDL.h>

namespace nom {

RendererInfo::RendererInfo ( void ) :
  flags_( 0 ),
  texture_width_( 0 ),
  texture_height_( 0 )
{
  //NOM_LOG_TRACE(NOM);
}

RendererInfo::~RendererInfo ( void )
{
  //NOM_LOG_TRACE(NOM);
}

const std::string& RendererInfo::name( void ) const
{
  return this->name_;
}

uint32 RendererInfo::optimal_texture_format ( void ) const
{
  return this->texture_formats.front();
}

bool RendererInfo::target_texture ( void ) const
{
  if ( this->flags_ & SDL_RENDERER_TARGETTEXTURE ) return true;

  return false;
}

bool RendererInfo::accelerated( void ) const
{
  if ( this->flags_ & SDL_RENDERER_ACCELERATED ) return true;

  return false;
}

bool RendererInfo::vsync( void ) const
{
  if ( this->flags_ & SDL_RENDERER_PRESENTVSYNC ) return true;

  return false;
}

int RendererInfo::texture_width( void ) const
{
  return this->texture_width_;
}

int RendererInfo::texture_height( void ) const
{
  return this->texture_height_;
}

std::ostream& operator << ( std::ostream& os, const RendererInfo& info )
{
  os << "Renderer Name: " << info.name()
  << std::endl << std::endl
  << "Device Capabilities: "
  << std::endl << std::endl;

  if ( info.target_texture() )
  {
    os << "SDL_RENDERER_TARGETTEXTURE"
    << std::endl;
  }

  if( info.accelerated() )
  {
    os << "SDL_RENDERER_ACCELERATED"
    << std::endl;
  }

  if( info.vsync() )
  {
    os << "SDL_RENDERER_PRESENTVSYNC"
    << std::endl;
  }

  os << std::endl
  << "Texture Formats: "
  << std::endl << std::endl;
  for ( nom::uint32 idx = 0; idx < info.texture_formats.size(); ++idx )
  {
    os << PIXEL_FORMAT_NAME( info.texture_formats[idx] )
    << std::endl;
  }
  os << "Texture Width: " << info.texture_width()
  << std::endl;
  os << "Texture Height: " << info.texture_height()
  << std::endl;

  return os;
}

} // namespace nom

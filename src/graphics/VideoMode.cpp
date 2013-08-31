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
#include "nomlib/graphics/VideoMode.hpp"

namespace nom {

VideoMode::VideoMode ( void ) : width ( 0 ), height ( 0 ), bpp ( 0 ) {}

VideoMode::VideoMode ( int32 mode_width, int32 mode_height, uint8 mode_bpp ) : width ( mode_width ), height ( mode_height ), bpp ( mode_bpp )
{}

VideoMode::~VideoMode ( void ) {}

void VideoMode::pp ( void )
{
  std::cout << std::endl << this->width << "x" << this->height << "x" << static_cast<int> ( this->bpp ) << std::endl << std::endl;
}

bool operator == ( const VideoMode& lhs, const VideoMode& rhs )
{
  return  ( lhs.width == rhs.width )                 &&
          ( lhs.height == rhs.height )               &&
          ( lhs.bpp == rhs.bpp );
}

bool operator != ( const VideoMode& lhs, const VideoMode& rhs )
{
  return ! ( lhs == rhs );
}

bool operator < ( const VideoMode& lhs, const VideoMode& rhs )
{
  if ( lhs.bpp == rhs.bpp )
  {
    if ( lhs.width == rhs.width )
    {
      return lhs.height < rhs.height;
    }
    else
    {
      return lhs.width < rhs.width;
    }
  }
  else
  {
    return lhs.bpp < rhs.bpp;
  }
}

bool operator > ( const VideoMode& lhs, const VideoMode& rhs )
{
  return rhs < lhs;
}

bool operator <= ( const VideoMode& lhs, const VideoMode& rhs )
{
  return ! (rhs < lhs );
}

bool operator >= ( const VideoMode& lhs, const VideoMode& rhs )
{
  return ! ( lhs < rhs );
}


} // namespace nom

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
#ifndef NOMLIB_SPRITE_HEADERS
#define NOMLIB_SPRITE_HEADERS

#include <iostream>
#include <string>
#include <utility>

#include "nomlib/config.hpp"
#include "nomlib/graphics/Canvas.hpp"
#include "nomlib/math/Transformable.hpp"

namespace nom {

enum class SpriteState
{
  Reserved = 0,
  Alive,
  Dying,
  Dead
};

class Sprite:
              public Transformable
{
  public:
    Sprite ( void );

    Sprite ( uint32 width, uint32 height );

    Sprite ( const Canvas& copy );

/* FIXME
    Sprite& operator = ( const Sprite& other );
*/

    virtual ~Sprite ( void );

    unsigned int getState ( void );
    void setState ( unsigned int state );

    signed int getSheetID ( void );
    void setSheetID ( signed int id );

    void setSheetDimensions (
                              int32 sheet_width, int32 sheet_height,
                              int32 spacing, int32 padding
                            );

    bool load ( std::string filename, Color colorkey,
                bool use_cache = 0,
                uint32 flags = SDL_SRCCOLORKEY | SDL_RLEACCEL
              );

    void Update ( void );
    void Draw ( void* video_buffer ) const;

    /// Rescale the font with a chosen resizing algorithm
    bool resize ( enum ResizeAlgorithm scaling_algorithm );

  private:
    Canvas sprite;
    unsigned int state; /// alive, dying, dead, ...

    Coords offsets;
    struct {
      signed int id; /// maps a specific sprite within sheet
      unsigned int sprite_width; /// width of sprite in sheet
      unsigned int sprite_height; /// height of sprite in sheet
      unsigned int width; /// width of sprite sheet
      unsigned int height; /// height of sprite sheet
      unsigned int spacing; /// applied between each sheet tile
      unsigned int padding; /// applied on all four sides of sheet tile
    } sheet;
};


} // namespace nom

#endif // NOMLIB_SPRITE_HEADERS defined

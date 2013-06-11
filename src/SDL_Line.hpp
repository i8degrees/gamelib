/******************************************************************************
    SDL_Line.hpp

  SDL Line Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_LINE_HEADERS
#define NOMLIB_SDL_LINE_HEADERS

#include <iostream>
#include <string>
#include <cmath>

#include "SDL.h"

#include "Coords.h"
#include "Color.h"
#include "SDL_Drawable.hpp"
#include "SDL_Pixel.hpp"
#include "Transformable.hpp"
#include "gamelib.h"

namespace nom
{
  class Line: public nom::SDL_Drawable,   // "is a" relationship
              public nom::Transformable   // "has a" relationship
  {
    public:
      Line ( void );
      virtual ~Line ( void );
      Line ( const nom::Coords& coords, const nom::Color& color );

      void Update ( void );
      void Draw ( void* video_buffer );
    private:
      nom::Pixel pixel;
  };
}

#endif // NOMLIB_SDL_LINE_HEADERS defined

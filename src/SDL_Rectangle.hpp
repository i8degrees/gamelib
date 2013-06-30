/******************************************************************************
    SDL_Rectangle.hpp

  SDL Rectangle Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_RECTANGLE_HEADERS
#define NOMLIB_SDL_RECTANGLE_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "Coords.hpp"
#include "Color.hpp"
#include "SDL_Drawable.hpp"
#include "Transformable.hpp"
#include "nomlib.hpp"

namespace nom
{
  class Rectangle:  public nom::SDL_Drawable,   // "is a" relationship
                    public nom::Transformable   // "has a" relationship

  {
    public:
      Rectangle ( void );
      Rectangle ( const nom::Rectangle& rect );
      Rectangle ( const nom::Coords& coords, const nom::Color& color );
      virtual ~Rectangle ( void );

      void Update ( void );
      void Draw ( void* ) const;
    private:
      //
  };
}

#endif // NOMLIB_SDL_RECTANGLE_HEADERS defined

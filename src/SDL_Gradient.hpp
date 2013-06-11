/******************************************************************************
    SDL_Gradient.hpp

  SDL Gradient effects

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef GAMELIB_SDL_GRADIENT_HEADERS
#define GAMELIB_SDL_GRADIENT_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "Coords.h"
#include "Color.h"
#include "SDL_Drawable.hpp"
#include "SDL_Rectangle.hpp"
#include "gfx.h"
#include "gamelib.h"

namespace nom
{
  class SDL_Gradient: //public nom::SDL_Drawable, // "is a" relationship
                      public nom::Transformable // "has a" relationship
  {
    public:
      SDL_Gradient ( void );
      virtual ~SDL_Gradient ( void );

      /// gradient[0] = starting color
      /// gradient[1] = ending color

      /// These default margin offsets are used in TTcards Info Boxes
      /// If they are not set here, they default to zero (0)

      void Init ( nom::Color starting_color, nom::Color ending_color,
                  unsigned int x = 0, unsigned int y = 0,
                  unsigned int width = 0, unsigned int height = 0,
                  unsigned int direction = 0,  unsigned int x_margin = 3,
                  unsigned int y_margin = 4 );

      /// direction { 0 } = ending color to starting color
      /// direction { 1 } = starting color to ending color

      void Draw ( void* video_buffer, unsigned int x, unsigned int y,
                  unsigned int width, unsigned int height, unsigned int direction = 0
                );

    private:
      nom::Color gradient[2]; /// holds R, G, B * 2
      nom::Coords coords; /// x, y, width, height coords
      uint32_t x_margin; /// x coordinate offset
      uint32_t y_margin; /// y coordinate offset
      uint32_t direction;
  };
}
/*

  nom::SDL_Gradient linear;

  nom::Color starting ( 66, 66, 66 );
  nom::Color ending ( 99, 99, 99 );

  linear.Init ( starting, ending, 104, 194, 176, 24, 0, 3, 4 );
  linear.Draw ( screen, 104, 194, 176, 24, 0 );

*/

#endif // GAMELIB_SDL_GRADIENT_HEADERS defined

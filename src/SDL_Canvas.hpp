/******************************************************************************
    SDL_Surface.h

    Abstraction of SDL Surfaces

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_SDL_SURFACE_HEADERS
#define NOMLIB_SDL_SURFACE_HEADERS

#include <iostream>
#include <string>
#include <cassert>

#include "ICanvas.hpp"

#include "SDL.h"

#include "gamelib.h"

#include "Color.h"
#include "Coords.h"
#include "gfx.h"
#include "SDL_Display.h"
#include "SDL_Image.hpp"
#include <memory> // for std::shared_ptr

namespace nom
{
  class SDL_Canvas
  {
    public:
      SDL_Canvas ( void );
      SDL_Canvas ( int32_t width, int32_t height, const Color& colorbit, uint32_t flags = 0 );
      SDL_Canvas ( void* video_buffer );
      ~SDL_Canvas ( void );

      void* get ( void );

      void setCanvas ( SDL_Surface *video_buffer );
      void setCanvas ( void* video_buffer );

      void setPosition ( const Coords& coords_ );
      void setOffsets ( const Coords& offsets_ );

      const int32_t getCanvasWidth ( void ) const;
      const int32_t getCanvasHeight ( void ) const;
      void* getCanvasPixelsFormat ( void ) const;

      bool loadImageFromFile  ( const std::string& filename, const nom::Color&
                                colorkey = nom::Color ( -1, -1, -1, -1 ),
                                uint32_t flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

      void Draw ( void* video_buffer );

    private:
      SDL_Surface *canvas_buffer;
      nom::Coords coords;
      nom::Coords offsets;
      nom::Color colorkey;
    };
}

#endif // NOMLIB_SDL_SURFACE_HEADERS defined

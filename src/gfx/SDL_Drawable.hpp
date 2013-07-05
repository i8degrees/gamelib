/******************************************************************************

    Drawable interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_DRAWABLE_HEADERS
#define NOMLIB_SDL_DRAWABLE_HEADERS

#include <iostream>

#include "gfx/IDrawable.hpp"
#include "nomlib_config.hpp"

namespace nom
{
  // This is an inheritance-only class
  class SDL_Drawable: public nom::IDrawable
  {
    public:
      virtual void Update ( void ) = 0;
      virtual void Draw ( void* ) const = 0;

      SDL_Drawable ( void )
      {
        #ifdef DEBUG_SDL_DRAWABLE_OBJ
          std::cout << "nom::SDL_Drawable::SDL_Drawable(): Hello, world!" << std::endl << std::endl;
        #endif
      }
      virtual ~SDL_Drawable ( void )
      {
        #ifdef DEBUG_SDL_DRAWABLE_OBJ
          std::cout << "nom::SDL_Drawable::~SDL_Drawable(): Goodbye cruel world!" << std::endl << std::endl;
        #endif
      }
  };
}

#endif // NOMLIB_SDL_DRAWABLE_HEADERS defined

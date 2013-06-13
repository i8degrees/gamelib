/******************************************************************************
    Transformable.hpp

  Inheritance class for adding Coords & Color methods

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_TRANSFORMABLE_HEADERS
#define NOMLIB_TRANSFORMABLE_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "Coords.hpp"
#include "Color.hpp"
#include "gamelib.h"

namespace nom
{
  class Transformable
  {
    public:
      Transformable ( void );
      virtual ~Transformable ( void );

      int32_t getX ( void ) const;
      int32_t getY ( void ) const;
      //void setX ( int32_t x );
      //void setY ( int32_t x );

      const nom::Coords getPosition ( void ) const;
      const nom::Coords getSize ( void ) const;
      void setPosition ( int32_t x, int32_t y );
      void setPosition ( int32_t x, int32_t y, int32_t width, int32_t height );
      void setPosition ( const nom::Coords& coords );
      void setSize ( int32_t width, int32_t height );

      void move ( int32_t x = 0, int32_t y = 0);

      const nom::Color& getColor ( void ) const;
      void setColor ( const nom::Color& color );
      void setColor ( uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255 );

    protected:
      nom::Coords coords;
      nom::Color color;
  };
}

#endif // NOMLIB_TRANSFORMABLE_HEADERS defined

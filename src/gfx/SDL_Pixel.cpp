/******************************************************************************

    SDL Point (Pixel) Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Pixel.hpp"

namespace nom {

Pixel::Pixel ( void )
{
  this->setPosition ( 0, 0 );
  this->setColor ( Color ( 0, 0, 0, 255 ) );
}

Pixel::~Pixel ( void )
{
  // ...
}

Pixel::Pixel ( const Coords& coords, const Color& color )
{
  this->setPosition ( coords );
  this->setColor ( color );
}

Pixel::Pixel ( int32_t x, int32_t y, const Color& color )
{
  this->setPosition ( x, y );
  this->setColor ( color );
}

void Pixel::Update ( void )
{
  // Stub
}

// 32-bit bpp
void Pixel::Draw ( void* video_buffer ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( video_buffer );

  uint32_t* pixels = ( uint32_t* ) buffer->pixels;

  uint32_t pixel = getColorAsInt ( buffer->format, this->color );

  pixels[ ( this->coords.y * buffer->w ) + this->coords.x ] = pixel;
}


} // namespace nom

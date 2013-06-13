/******************************************************************************
    SDL_Rectangle.cpp

  SDL Rectangle Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Rectangle.hpp"

nom::Rectangle::Rectangle ( void )
{
  this->coords = nom::Coords ( 0, 0, 0, 0 );
  this->color = nom::Color ( 0, 0, 0 );
}

nom::Rectangle::~Rectangle ( void )
{
  // ...
}

// Constructor variant for creating a rectangle from an existing type
// This is a "shallow" copy assignment
nom::Rectangle::Rectangle ( const nom::Rectangle& rect )
{
  this->coords = rect.coords;
  this->color = rect.color;
}

nom::Rectangle::Rectangle ( const nom::Coords& coords, const nom::Color& color )
{
  this->coords = coords;
  this->color = color;
}

void nom::Rectangle::Update ( void )
{
  // Stub
}

void nom::Rectangle::Draw ( void* video_buffer ) const
{
  SDL_Rect rectangle = this->coords.getSDL_Rect();
  unsigned int rectangle_color = 0;
  SDL_Surface *buffer = (SDL_Surface*) video_buffer;

  rectangle_color = this->color.getColorAsInt ( buffer->format );

  if ( SDL_FillRect ( buffer, &rectangle, rectangle_color ) != 0 )
  {
    std::cout << "ERR in nom::Rectangle::Draw(): " << SDL_GetError() << std::endl;
  }
}

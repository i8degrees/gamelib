/******************************************************************************
    Transformable.cpp

  Inheritance class for adding Coords & Color methods

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Transformable.hpp"

nom::Transformable::Transformable ( void )  : coords ( 0, 0, 0, 0 ),
                                              color ( 0, 0, 0, -1 )
{
  #ifdef DEBUG_TRANSFORMABLE_OBJ
    std::cout << "Transformable::Transformable (): " << "Hello, world!" << std::endl << std::endl;
  #endif
}

nom::Transformable::~Transformable ( void )
{
  #ifdef DEBUG_TRANSFORMABLE_OBJ
    std::cout << "Transformable::~Transformable (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  // Cleanup vars
}

int32_t nom::Transformable::getX ( void ) const
{
  return this->coords.getX();
}

int32_t nom::Transformable::getY ( void ) const
{
  return this->coords.getY();
}

const nom::Coords& nom::Transformable::getXY ( void ) const
{
  return this->coords;
}

void nom::Transformable::setX ( int32_t x )
{
  this->coords.setX ( x );
}

void nom::Transformable::setY ( int32_t y )
{
  this->coords.setY ( y );
}

void nom::Transformable::setXY ( int32_t x, int32_t y )
{
  this->coords.setXY ( x, y );
}

int32_t nom::Transformable::getWidth ( void ) const
{
  return this->coords.getWidth();
}

void nom::Transformable::setWidth ( int32_t width, int32_t height )
{
  this->coords.setWidth ( width );
}

int32_t nom::Transformable::getHeight ( void ) const
{
  return this->coords.getHeight();
}

void nom::Transformable::setHeight ( int32_t height )
{
  this->coords.setHeight ( height );
}

void nom::Transformable::updateXY ( int32_t x, int32_t y )
{
  int32_t x_offset = this->coords.getX();
  int32_t y_offset = this->coords.getY();

  x_offset += x;
  y_offset += y;
  this->coords.setXY ( x_offset, y_offset );
}

const nom::Color& nom::Transformable::getColor ( void ) const
{
  return this->color;
}

void nom::Transformable::setColor ( const nom::Color& color_ )
{
  this->color = color_;
}


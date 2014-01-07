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
#include "nomlib/gui/MessageBox.hpp"

namespace nom {
  namespace ui {

MessageBox::MessageBox ( void ) :
  enabled_ ( true )
{
  NOM_LOG_TRACE(NOM);
}

MessageBox::~MessageBox ( void )
{
  NOM_LOG_TRACE ( NOM );
}

MessageBox::MessageBox  (
                          int32 x, int32 y, int32 width, int32 height,
                          enum FrameStyle style,
                          const Gradient& background
                        )
{
  int32 padding = 1;

  this->enable();

  // init geometry coords w/ arguments list
  this->coords = Coords ( x, y, width, height );

  Color4u gradient_color[2];
  gradient_color[0] = background.start_color();
  gradient_color[1] = background.end_color();

  this->drawable.push_back ( Gradient::SharedPtr ( new Gradient ( gradient_color, Coords ( this->coords.x, this->coords.y, this->coords.w, this->coords.h ), 0, 0, background.fill_direction() ) ) );

  if ( style == FrameStyle::Gray )
  {
    this->drawable.push_back ( GrayFrame::SharedPtr ( new GrayFrame ( x, y, width, height, padding ) ) );
  }
}

MessageBox::MessageBox  (
                          int32 x, int32 y, int32 width, int32 height,
                          GrayFrame::SharedPtr style,
                          Gradient::SharedPtr background
                        )
{
  int32 padding = 1;

  this->enable();

  // init geometry coords w/ arguments list
  this->coords = Coords ( x, y, width, height );

  Color4u gradient_color[2];
  gradient_color[0] = background->start_color();
  gradient_color[1] = background->end_color();
  this->drawable.push_back ( Gradient::SharedPtr ( new Gradient ( gradient_color, Coords ( this->coords.x, this->coords.y, this->coords.w, this->coords.h ), 0, 0, background->fill_direction() ) ) );

  if ( style != nullptr )
  {
    style->setPosition ( x, y );
    style->setSize ( width, height, padding );
    this->drawable.push_back ( style );
  }
  else // default frame style
  {
    this->drawable.push_back ( GrayFrame::SharedPtr ( new GrayFrame ( x, y, width, height, padding ) ) );
  }
}

bool MessageBox::enabled ( void ) const
{
  if ( this->enabled_ == false ) return false;

  return true;
}

const std::string MessageBox::title ( void )
{
  return this->mbox_title_.text();
}

const std::string MessageBox::text ( void )
{
  return this->mbox_text_.text();
}

const Point2i MessageBox::size ( void ) const
{
  return Point2i ( this->coords.w, this->coords.h );
}

const Point2i MessageBox::position ( void ) const
{
  return Point2i ( this->coords.x, this->coords.y );
}

void MessageBox::disable ( void )
{
  this->enabled_ = false;
}

void MessageBox::enable ( void )
{
  this->enabled_ = true;
}

void MessageBox::set_title ( Label& title )
{
  this->mbox_title_ = title;

  // This positions the title text of the message box on top of the second "top"
  // bordering color of GrayFrame, commented as "top1".
  //
  // The original coords.x value was + 8, but I think + 4 looks best.
  this->mbox_title_.set_position ( IntRect ( this->coords.x + 4, this->coords.y, this->coords.w, this->coords.h ) );

  this->mbox_title_.set_text ( "INFO." );
  this->mbox_title_.set_alignment ( Label::TextAlignment::MiddleLeft );

  this->drawable.push_back ( IDrawable::SharedPtr ( new Label ( this->mbox_title_ ) ) );
}

void MessageBox::set_text ( Label& text )
{
  this->mbox_text_ = text;

  this->mbox_text_.set_position ( IntRect ( this->coords.x, this->coords.y, this->coords.w, this->coords.h ) );
  this->mbox_text_.set_alignment ( Label::MiddleCenter );

  this->drawable.push_back ( IDrawable::SharedPtr ( new Label ( this->mbox_text_ ) ) );
}

void MessageBox::update ( void )
{
  for ( auto it = this->drawable.begin(); it != this->drawable.end(); ++it )
  {
    IDrawable::SharedPtr obj = *it;
    obj->update();
  }
}

void MessageBox::draw ( RenderTarget target ) const
{
  if ( this->enabled() == false ) return;

  for ( auto it = this->drawable.begin(); it != this->drawable.end(); ++it )
  {
    IDrawable::SharedPtr obj = *it;
    obj->draw ( target );
  }
}

} // namespace ui
} // namespace nom

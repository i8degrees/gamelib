/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include "nomlib/gui/Button.hpp"

namespace nom {

Button::Button( void )
{
  // NOM_LOG_TRACE( NOM );
}

Button::~Button( void )
{
  // NOM_LOG_TRACE( NOM );
}

Button::Button  (
                  UIWidget* parent,
                  int64 id,
                  const Point2i& pos,
                  const Size2i& size
                ) :
  UIWidget( parent, id, pos, size )   // Base class
{
  // NOM_LOG_TRACE( NOM );

  // Use explicitly set coordinates for our minimum widget size
  this->set_minimum_size( size );

  // Auto-generate a name tag for our widget.
  this->set_name( "button" );

  NOM_CONNECT_UIEVENT( this, UIEvent::ON_WINDOW_SIZE_CHANGED, this->on_size_changed );
}

const Size2i Button::size_hint( void ) const
{
  // Total text height requirements for stored items
  sint total_text_height = 0;

  // Default point size of font
  uint point_size = 12;

  // Calculate the total text height requirements for the widget.
  FontMetrics face = this->label_.font()->metrics();

// NOM_DUMP( this->label_.text() );
// NOM_DUMP( face.name );

  // Use the point size of font used instead of initialized default
  point_size = this->label_.text_size();

  // Maximum pixel height of the font's glyph
  total_text_height += this->label_.font()->newline( point_size );

// NOM_DUMP( total_text_height );

  // If we have calculated a total text height requirement, we can stop here,
  // using the total text height for our preferred height field.
  if( total_text_height > 0 )
  {
    return Size2i( this->size().w, total_text_height );
  }

  // We do not have any text labels stored, so assume a widget with a height
  // large enough for one item.
  if( this->font().valid() == true )
  {
// NOM_DUMP( this->font()->newline( point_size ) );

    // FIXME: Determine what our default font point size, if any, should be;
    // Assuming a default of 12 pt for the time being...
    return Size2i( this->size().w, this->font()->newline( point_size ) );
  }

  // If all else fails ... use the preset size of the widget as the preferred
  // size.
  //
  // Should we be using minimum_size here instead?
  return Size2i( this->size() );
}

ObjectTypeInfo Button::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

Button::SelfType& Button::operator =( const SelfType& rhs )
{
  this->label_ = rhs.label_;
  this->text_ = rhs.text_;
  this->point_size_ = rhs.point_size_;
  this->alignment_ = rhs.alignment_;
  this->text_color_ = rhs.text_color_;

  // this->update();

  return *this;
}

bool Button::valid( void ) const
{
  if ( this->position() != Point2i::null || this->size() != Size2i::null )
  {
    return true;
  }

  return false;
}

void Button::update( void )
{
  UIWidget::update();

  if( this->updated() ) return;

  this->label_ = Text( this->text_, this->font(), this->point_size_, this->alignment_, this->text_color_ );
  this->label_.set_position( this->position() );
  this->label_.set_size( this->size() );
  this->label_.set_alignment( this->alignment_ );

  this->set_updated( true );
}

void Button::draw( RenderTarget& target ) const
{
  UIWidget::draw( target );

  // NOTE: Validity is checked before attempting to render.
  this->label_.draw( target );
}

bool Button::process_event( const nom::Event& ev )
{
  Point2i mouse_coords( ev.mouse.x, ev.mouse.y );

  UIWidget::process_event( ev );

  // FIXME (?):
  //
  // if( UIWidget::process_event( ev ) ) return true;

  // Registered action for mouse button events
  if( ev.type == SDL_MOUSEBUTTONUP )
  {
    IntRect label_bounds( this->label_.global_bounds() );

    if( label_bounds.contains( mouse_coords ) )
    {
      // Send the index and string text of the label that was selected during
      // the time of this event.
      UIWidgetEvent item_ev( 0, this->label_.text(), ev );

      item_ev.set_id( this->id() );

      // Send the UI event object to the registered callback.
      this->dispatcher()->emit( UIEvent::MOUSE_UP, item_ev );

      // Successfully processed events
      return true;
    } // end if label text are within the bounds of mouse coordinates
  } // end if event type == SDL_MOUSE_BUTTONUP

  // No processed events
  return false;
}

const std::string& Button::label_text( void ) const
{
  return this->label_.text();
}

void Button::set_label( const std::string& text, uint point_size, enum Text::Alignment align, const Color4i& color )
{
  this->text_ = text;
  this->point_size_ = point_size;
  this->alignment_ = align;
  this->text_color_ = color;

  this->set_updated( false );

  this->update();
}

// Protected scope

void Button::on_size_changed( const UIWidgetEvent& ev )
{
  Event evt = ev.event();

  if( evt.type != SDL_WINDOWEVENT_SIZE_CHANGED )
  {
    return;
  }

  if( this->decorator() )
  {
    this->set_updated( false );

    // Update the attached decorator (border & possibly a background)
    this->decorator()->set_bounds( ev.resized_bounds_ );
  }

  // Update ourselves with the new rendering coordinates
  this->set_bounds( ev.resized_bounds_ );

  this->update();
}

} // namespace nom
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
#include "nomlib/gui/QuestionDialogBox.hpp"

// Forward declarations
#include "nomlib/gui/UIEventDispatcher.hpp"
#include "nomlib/gui/UIWidgetEvent.hpp"

namespace nom {

QuestionDialogBox::QuestionDialogBox  (
                                        UIWidget* parent,
                                        int64 id,
                                        const Point2i& pos,
                                        const Size2i& size
                                      ) :
  MessageBox( parent, id, pos, size ),
  // selection_{ -1 },
  selection_{ 0 }
{
  // NOM_LOG_TRACE( NOM );

  // Use explicitly set coordinates for our minimum widget size
  this->set_minimum_size( size );

  // Auto-generate a name tag for our widget.
  this->set_name( "question_box" );

  // Default highlighted item text color.
  this->set_selected_text_color( Color4i::Red );

  // Initialize the default event listeners for the widget.
  // NOM_CONNECT_UIEVENT( this, UIEvent::ON_WINDOW_SIZE_CHANGED, this->on_size_changed );

  // this->update();
}

QuestionDialogBox::~QuestionDialogBox( void )
{
  // NOM_LOG_TRACE( NOM );
}

int QuestionDialogBox::hit_test( const Point2i& pt )
{
  // Counter for the position of each element; must start at zero and count our
  // way up to the last element to figure out where we are in the array.
  int index = 0;

  for( auto itr = this->choices_.begin(); itr != this->choices_.end(); ++itr )
  {
    if( this->contains( itr->get(), pt ) == true )
    {
      // Found our array / element position within the internal storage
      // container!
      return index;
    }

    // Increment element position
    ++index;
  }

  // No position found
  return npos;
}

void QuestionDialogBox::draw( RenderTarget& target ) const
{
  // First, render our base class
  MessageBox::draw( target );

  for( auto itr = this->choices_.begin(); itr != this->choices_.end(); ++itr )
  {
    if( (*itr).valid() == true )
    {
      (*itr).draw( target );
    }
  }
}

void QuestionDialogBox::append_choice( const std::string& label )
{
  Text choice;

  choice.set_font( this->font() );
  choice.set_text_size( nom::DEFAULT_FONT_SIZE );
  choice.set_text( label );
  choice.set_position( this->position() );
  choice.set_size( this->size() );
  choice.set_alignment( Text::Alignment::MiddleCenter );

  this->choices_.push_back( choice );

  this->update();
}

void QuestionDialogBox::set_selected_text_color( const Color4i& color )
{
  this->selected_text_color_ = color;

  this->update();
}

// Protected scope

void QuestionDialogBox::update( void )
{
  // Elapsed element position counter
  int index = 0;

  Point2i pos( this->position().x, this->position().y );

  if( this->choices_.empty() ) return;

  // FIXME: This call is necessary for updating the geometries of the base
  // class.
  MessageBox::update();

  for( auto itr = this->choices_.begin(); itr != this->choices_.end(); ++itr )
  {
    (*itr).set_font( this->font() );

    (*itr).set_position( pos );
    (*itr).set_size( Size2i( this->size() ) );

    pos.x = pos.x;
    pos.y += (*itr).height();

    // Make the current item label selection stand out by marking it a different
    // color.
    if( this->selection() == index )
    {
      (*itr).set_color( this->selected_text_color() );
    }
    else
    {
      (*itr).set_color( Color4i::White );
    }

    // ...Next element position!
    ++index;
  }
}

void QuestionDialogBox::on_size_changed( const UIWidgetEvent& ev )
{
  NOM_STUBBED( NOM );
}

void QuestionDialogBox::on_mouse_down( const UIWidgetEvent& ev )
{
  int index = 0;
  Event event = ev.event();

  // Let our base class handle its share of the events -- the title caption &
  // message text bounds.
  //
  // The choice text labels are ours to handle and should not overlap whatsoever
  // with MessageBox.
  MessageBox::on_mouse_down( ev );

  // Registered action for mouse button event
  if( event.type == SDL_MOUSEBUTTONDOWN )
  {
    UIEvent evcode[2];
    UIWidgetEvent wev;
    Point2i mouse( event.mouse.x, event.mouse.y );
    index = this->hit_test( mouse );

    if( index != npos )
    {
      // TODO:
      // this->set_focused( true );

      // Send the array index in our event; this signifies which choice was
      // selected.
      wev.set_index( index );

      // FIXME:
      // Send the text of the selection.
      wev.set_text( this->choice( index ) );

      // Set the current index position to the selected text label -- this
      // has the side effect of updating the text color; see also: ::update.
      this->set_selection( index );

      // Set the associated nom::Event object for this UI event.
      wev.set_event( event );

      wev.set_id( this->id() );

      // Single mouse click event
      if( event.mouse.clicks < 2 )
      {
        evcode[0] = UIEvent::ON_MOUSE_DOWN;   // Private event
        evcode[1] = UIEvent::MOUSE_DOWN;      // Public event
      }
      // Double mouse click event
      else
      {
        evcode[0] = UIEvent::ON_MOUSE_DCLICK;   // Private event
        evcode[1] = UIEvent::MOUSE_DCLICK;      // Public event
      }

      // Send the UI event object to the registered callback; this is the
      // "private" interface -- reserved for internal class implementations.
      this->dispatcher()->emit( evcode[0], wev );

      // Send the UI event object to the registered callback; this is the
      // event that gets heard by any end-user listening in, unlike the
      // private message above.
      this->dispatcher()->emit( evcode[1], wev );

      this->set_selected_text_color( this->selected_text_color() );
    }
  } // end if event type == SDL_MOUSEBUTTONDOWN
}

void QuestionDialogBox::on_mouse_up( const UIWidgetEvent& ev )
{
  // Stub
}

void QuestionDialogBox::on_mouse_wheel( const UIWidgetEvent& ev )
{
  int index = 0;
  Event event = ev.event();

  // Registered action for mouse button event
  if( event.type == SDL_MOUSEWHEEL )
  {
    UIWidgetEvent wev;

    // TODO:
    if( this->focused() == false )
    {
      // return;
    }

    // Counter for the position of each element; must start at current
    // selection.
    index = this->selection();

    // Send the array index in our event; this signifies which choice was
    // selected.
    wev.set_index( index );

    // Set the current index position to the selected text label -- this
    // has the side effect of updating the text color; see also: ::update.
    this->set_selection( index );

    // Send the text of the selection if there is a selection set. Note that
    // this needs to be set *after* the selection has been set.
    if( index != npos )
    {
      wev.set_text( this->choice( index ) );
    }
    else
    {
      wev.set_text( "\0" );
    }

    // Set the associated nom::Event object for this UI event.
    wev.set_event( event );

    wev.set_id( this->id() );

    // Send the UI event object to the registered callback; this is the private
    // event interface.
    this->dispatcher()->emit( UIEvent::ON_MOUSE_WHEEL, wev );

    // Send the UI event object to the registered callback; this is the public
    // event interface.
    this->dispatcher()->emit( UIEvent::MOUSE_WHEEL, wev );

    // Default internal reaction to the event action (SDL_MOUSEWHEEL):

    // Our internal storage container could also be used to obtain the
    // selection index.
    int selected = this->selection();

    // Up
    if( event.wheel.y > 0 && selected > 0 )
    {
      --selected;
      this->set_selection( selected );
      // NOM_DUMP(this->selection() );
    }

    // Down
    else if( event.wheel.y < 0 && selected < this->choices_.size() - 1 )
    {
      ++selected;
      this->set_selection( selected );
      // NOM_DUMP(this->selection() );
    }

    // NOM_DUMP(ev.index() );
    // NOM_DUMP(ev.text() );

    this->update();
  } // end if event type == SDL_MOUSEWHEEL
}

void QuestionDialogBox::on_key_down( const UIWidgetEvent& ev )
{
  int index = 0;
  nom::Event event = ev.event();
  UIWidgetEvent wev;

  // Registered action for key press event
  if( event.type == SDL_KEYDOWN )
  {
    // TODO:
    if( this->focused() == false )
    {
      // return;
    }

    // Position of each item
    index = this->selection();

    // Send the array index in our event; this signifies which choice was
    // selected.
    wev.set_index( index );

    // Set the current index position to the selected text label -- this
    // has the side effect of updating the text color; see also: ::update.
    this->set_selection( index );

    // Send the text of the selection if there is a selection set. Note that
    // this needs to be set *after* the selection has been set.
    if( index != npos )
    {
      wev.set_text( this->choice( index ) );
    }
    else
    {
      wev.set_text( "\0" );
    }

    // Set the associated nom::Event object for this UI event.
    wev.set_event( event );

    wev.set_id( this->id() );

    // Send the UI event object to the registered callback; this is the private
    // event interface.
    this->dispatcher()->emit( UIEvent::ON_KEY_DOWN, wev );

    // Send the UI event object to the registered callback; this is the public
    // event interface.
    this->dispatcher()->emit( UIEvent::KEY_DOWN, wev );

    // Default internal reaction to the event action (SDL_KEYDOWN):

    // Our internal storage container could also be used to obtain the
    // selection index.
    int selected = this->selection();

    if( event.key.sym == SDLK_UP && selected > 0 )
    {
      --selected;

      this->set_selection( selected );
      // NOM_DUMP(this->selection() );
    }
    else if( event.key.sym == SDLK_DOWN && ( selected < this->choices_.size() - 1 ) )
    {
      ++selected;

      this->set_selection( selected );
      // NOM_DUMP(this->selection() );
    }
    else
    {
      // NOM_DUMP( selected );
    }

    // NOM_DUMP(ev.index());
    // NOM_DUMP(ev.text());

    this->update();

  } // end if event type == SDL_KEYDOWN
}

void QuestionDialogBox::on_key_up( const UIWidgetEvent& ev )
{
  // Stub
}

int QuestionDialogBox::selection( void ) const
{
  return this->selection_;
}

uint QuestionDialogBox::items_size( void ) const
{
  return this->choices_.size();
}

QuestionDialogBox::ItemStrings QuestionDialogBox::choices( void ) const
{
  ItemStrings label_texts;

  for( auto itr = this->choices_.begin(); itr != this->choices_.end(); ++itr )
  {
    label_texts.push_back( (*itr).text() );
  }

  return label_texts;
}

std::string QuestionDialogBox::choice( int pos ) const
{
  int idx = 0;
  for( auto itr = this->choices_.begin(); itr != this->choices_.end(); ++itr )
  {
    if( pos == idx )
    {
      return itr->text();
    }

    ++idx;
  }

  return "\0";
}

const Color4i& QuestionDialogBox::selected_text_color( void ) const
{
  return this->selected_text_color_;
}

void QuestionDialogBox::set_selection( int pos )
{
  this->selection_ = pos;

  // this->update();
}

} // namespace nom

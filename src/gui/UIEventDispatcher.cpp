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
#include "nomlib/gui/UIEventDispatcher.hpp"

namespace nom {

UIEventDispatcher::UIEventDispatcher( void )
{
  // NOM_LOG_TRACE( NOM );
}

UIEventDispatcher::~UIEventDispatcher( void )
{
  // NOM_LOG_TRACE( NOM );
}

UIEventDispatcher::UIEventDispatcher( const self_type& rhs )
{
  // NOM_LOG_TRACE( NOM );

  this->set_observers( rhs.observers() );
}

UIEventDispatcher::self_type& UIEventDispatcher::operator =( const self_type& rhs )
{
  // NOM_LOG_TRACE( NOM );

  this->set_observers( rhs.observers() );

  return *this;
}

bool UIEventDispatcher::register_event_listener( const event_type& ev, const callback_type& observer )
{
  this->observers_[ev].push_back( observer );

  return true;
}

bool UIEventDispatcher::remove_event_listener( const event_type& key )
{
  // The referenced event is our lookup key.
  auto res = this->observers_.find( key );

  // Match found; commence the erasure of the delegate referenced by event.
  if( res != this->observers_.end() )
  {
    this->observers_.erase( res->first );

    return true;
  }
  else  // No match found; nothing to remove!
  {
    return false;
  }

  return false;
}

uint32 UIEventDispatcher::size( void ) const
{
  return this->observers_.size();
}
bool UIEventDispatcher::emit( const event_type& ev, UIWidgetEvent& data ) const
{
  auto res = this->observers_.find( ev );

  // Match found
  if( res != this->observers_.end() )
  {
    auto obs = res->second;
    for( auto itr = obs.begin(); itr != obs.end(); ++itr )
    {
      // Match found; execute callback.
      itr->operator()( data );

      // Log matched event after the callback execution (dump of a value).
      //
      // Note that we could also print the value of res->first here.
      // NOM_LOG_INFO( NOM, "notify match: " + std::to_string( /*ev*/-1 ) );

      // Notification event
      return true;
    }
  }

  // No match found; do nothing.
  else
  {
    // NOM_LOG_INFO( NOM, "notify != match: " + std::to_string( /*ev*/-1 ) );

    // No notification event
    return false;
  }

  // No notification event
  return false;
}

bool UIEventDispatcher::find( const event_type& key ) const
{
  // The referenced event is our lookup key.
  auto res = this->observers_.find( key );

  if( res != this->observers_.end() )
  {
    // Match found
    return true;
  }
  else
  {
    // No match found.
    return false;
  }

  return false;
}

// Private scope

const UIEventDispatcher::CallbackTable& UIEventDispatcher::observers( void ) const
{
  return this->observers_;
}

void UIEventDispatcher::set_observers( const CallbackTable& ev_table )
{
  this->observers_ = ev_table;
}

} // namespace nom

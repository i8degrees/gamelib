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
#include "nomlib/system/EventDispatcher.hpp"

namespace nom {

EventDispatcher::EventDispatcher ( void )
{
NOM_LOG_TRACE ( NOM );
}

EventDispatcher::~EventDispatcher ( void )
{
NOM_LOG_TRACE ( NOM );
}

int32 EventDispatcher::push ( SDL_Event* event, int32 code, void* params )
{
  SDL_UserEvent user_event;

  user_event.type = SDL_USEREVENT;
  user_event.code = code;
  user_event.data1 = params;
  user_event.data2 = nullptr;

  event->type = SDL_USEREVENT;
  event->user = user_event;

  if ( SDL_PushEvent ( event ) != 1 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return -1;
  }

  return 0;
}

int32 EventDispatcher::dispatch ( enum UserEvent code, void* params )
{
  SDL_Event event;

  if ( this->push ( &event, static_cast<int32> ( code ), nullptr ) != 0 )
  {
NOM_LOG_ERR ( NOM, "Could not dispatch event." );
    return -1;
  }

  return 0;
}


} // namespace nom

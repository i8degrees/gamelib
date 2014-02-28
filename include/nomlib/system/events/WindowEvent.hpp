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
#ifndef NOMLIB_SYSTEM_EVENTS_WINDOW_EVENT_HPP
#define NOMLIB_SYSTEM_EVENTS_WINDOW_EVENT_HPP

// #include "SDL.h"

#include "nomlib/config.hpp"

namespace nom {

/// \brief A structure containing information on a window event.
struct WindowEvent
{
  /// \brief The event type.
  ///
  /// \remarks SDL_WINDOWEVENT.
  uint32 type;

  /// \brief An enumeration of window events.
  ///
  /// \remarks SDL_WINDOWEVENT_NONE (not used), SDL_WINDOWEVENT_SHOWN,
  /// SDL_WINDOWEVENT_HIDDEN, ..., SDL_WINDOWEVENT_CLOSE.
  uint8 event;

  /// \brief Event dependent data; typically the X coordinate position or width
  /// data of an event.
  int32 data1;

  /// \brief Event dependent data; typically the Y coordinate position or height
  /// data of an event.
  int32 data2;

  /// \brief The identifier of the window at the moment of the event.
  uint32 window_id;

  /// \brief The recorded time at the moment of the event.
  uint32 timestamp;
};

} // namespace nom

#endif // include guard defined

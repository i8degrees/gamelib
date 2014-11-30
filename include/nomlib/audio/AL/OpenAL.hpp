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
#ifndef NOMLIB_OPENAL_HEADERS
#define NOMLIB_OPENAL_HEADERS

#include <iostream>
#include <string>
#include <ctime>

#include "nomlib/config.hpp"
#include "nomlib/core/clock.hpp"

#if defined(NOM_PLATFORM_OSX) // Use platform-distributed OpenAL headers
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
#else // As per the header inclusion path that FindOpenAL.cmake offers us
  #include <al.h>
  #include <alc.h>
#endif

/// Optimal sound frame size (in bytes); used by libsndfile
const nom::uint32 BUFFER_SIZE = 4096;
const nom::uint32 NUM_SOURCES = 16; // not implemented

const float MIN_VOLUME = 0.0;
const float MAX_VOLUME = 100.0;

#ifdef NOM_DEBUG
  #define AL_CHECK_ERR(Function) \
    ( (Function ), nom::priv::al_err ( __FILE__, __LINE__ ) )
#else
  #define AL_CHECK_ERR(Function) ( Function )
#endif

namespace nom {
  namespace priv {

void al_err ( const std::string& file, uint32 line );

  } // namespace priv
} // namespace nom


#endif // NOMLIB_OPENAL_HEADERS defined

/// \class nom::OpenAL
/// \ingroup audio
///
/// \see http://ffainelli.github.io/openal-example/

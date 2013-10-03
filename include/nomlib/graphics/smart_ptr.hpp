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
#ifndef NOMLIB_SMART_PTR_HELPERS_HPP
#define NOMLIB_SMART_PTR_HELPERS_HPP

#include <memory>

#include "SDL.h"

#include "nomlib/config.hpp"

namespace nom {
  namespace priv {

/// Custom deleter for SDL_Window struct
void FreeWindow ( SDL_Window* );

/// Custom deleter for SDL_Renderer struct; function overload for
/// std::shared_ptr
void FreeRenderTarget ( SDL_Renderer* );

/// Custom deleter for SDL_Renderer struct; function overload for
/// std::unique_ptr
void FreeRenderTarget ( void* renderer );

/// Custom deleter for SDL_Surface struct
void FreeSurface ( SDL_Surface* );

/// Custom deleter for SDL_Texture struct
void FreeTexture ( SDL_Texture* );


  } // namespace priv
} // namespace nom

#endif // include guard defined

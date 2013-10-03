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
#ifndef NOMLIB_EXAMPLES_SDL_APP_HPP
#define NOMLIB_EXAMPLES_SDL_APP_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>

/// Installation prefix of our application.
///
/// Defaults to a null terminated string -- no prefix -- which leaves us in the
/// file path origin wherever we are executed from.
const std::string APP_INSTALL_PREFIX = "\0";

/// File path name of the resources directory; this must be a relative file path.
const std::string APP_RESOURCES_DIR = "Resources";

/// Name of our application.
const std::string APP_NAME = "nom::SDL2 Demo - Window";

/// Width, in pixels, of our effective rendering surface.
constexpr nom::int32 WINDOW_WIDTH = 768;

/// Height, in pixels, of our effective rendering surface.
constexpr nom::int32 WINDOW_HEIGHT = 448;

/// Maximum number of active windows we will attempt to spawn in this example
constexpr nom::int32 MAXIMUM_WINDOWS = 3;

/// Relative file path name of our resource example
///
/// FIXME
/// This is a temporary fix until we can use IMG_Load for loading the PNG image 
/// again.
/// (See FIXME notes inside Display.hpp at setWindowIcon method for details).
/// Jeffrey Carpenter <jeffrey.carp@gmail.com> @ 2013-10-01
#if defined ( NOM_PLATFORM_WINDOWS )
  const std::string RESOURCE_ICON = "icon.bmp";
#else // assume POSIX platform
  const std::string RESOURCE_ICON = "icon.png";
#endif


/// \brief Usage example
class App:
            public nom::SDL_App
{
  public:
    App ( nom::int32 args_count, char* args[] );
    ~App ( void );

    bool onInit ( void );
    nom::int32 Run ( void );

  private:
    void onKeyDown ( nom::int32 key, nom::int32 mod );

    /// Window handles
    nom::Window window[MAXIMUM_WINDOWS];

    /// Interval at which we refresh the frames per second counter
    nom::Timer update[MAXIMUM_WINDOWS];

    /// Timer for tracking frames per second
    nom::FPS fps[MAXIMUM_WINDOWS];

    /// Input events
    SDL_Event event;
};

#endif // include guard defined

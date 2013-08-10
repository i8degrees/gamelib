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
#ifndef NOMLIB_SDL_MESSAGEBOX_HEADERS
#define NOMLIB_SDL_MESSAGEBOX_HEADERS

#include <iostream>
#include <string>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/graphics/Gradient.hpp"
#include "nomlib/graphics/Line.hpp"
#include "nomlib/gui/IFrame.hpp"
#include "nomlib/gui/GrayFrame.hpp"

namespace nom {
  namespace ui {

enum FrameStyle
{
  None = 0,
  Gray = 1
};

/// \brief Simple UI interface for drawing a styled message box
class MessageBox: public IDrawable,     // "is a" relationship
                  public Transformable  // "has a" relationship
{
  public:
    MessageBox ( void );

    MessageBox  ( int32 x, int32 y, int32 width, int32 height,
                  enum FrameStyle, const Gradient& background
                );

    MessageBox  ( int32 x, int32 y, int32 width, int32 height,
                  std::shared_ptr<IFrame> style = nullptr,
                  const Gradient& background = Gradient()
                );

    virtual ~MessageBox ( void );

    bool isEnabled ( void );
    void disable ( void );
    void enable ( void );

    void Update ( void );
    void Draw ( void* video_buffer ) const;

  private:
    void initialize ( void );
    std::shared_ptr<IFrame> frame;
    Gradient background;
    bool enabled;
};


  } // namespace ui
} // namespace nom

#endif // NOMLIB_SDL_MESSAGEBOX_HEADERS defined

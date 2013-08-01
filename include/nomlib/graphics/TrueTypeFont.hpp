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
#ifndef NOMLIB_SDL_TRUETYPE_FONT_HEADERS
#define NOMLIB_SDL_TRUETYPE_FONT_HEADERS

#include <iostream>
#include <string>
#include <memory>

#include <SDL/SDL_ttf.h>

#include "nomlib/config.hpp"
#include "nomlib/system/Logger.hpp"
#include "nomlib/graphics/IFont.hpp"
#include "nomlib/sdl/utils.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/graphics/Canvas.hpp"
//#include "nomlib/graphics/IDrawable.hpp"

namespace nom {
  namespace priv {

/// Custom deleter for TTF_Font* smart pointers; can be used as a debugging aid.
void TTF_FreeSurface ( TTF_Font* );

  } // namespace priv
} // namespace nom


namespace nom {

class TrueTypeFont: //  public IDrawable, //  "is a" inheritance
                    public IFont // "is-a" relationship
{
  public:
    /// Default constructor; we initialize the SDL_ttf extension here
    TrueTypeFont ( void );

    /// Default destructor; we shutdown the SDL_ttf extension here
    ~TrueTypeFont ( void );

    /// Is this object initialized -- not nullptr?
    bool valid ( void ) const;

    /// Obtains set text string buffer; defaults to \0
    const std::string& getText ( void ) const;

    /// Compute the width in pixels of the set text string; defaults to zero (0)
    int32 getFontWidth ( void ) const;

    /// Compute the height in pixels of the set text string; defaults to zero (0)
    int32 getFontHeight ( void ) const;

    FontStyle getFontStyle ( void ) const;

    void setFontStyle ( uint8 style, uint8 options );

    /// Set a new text point size
    void setFontSize ( int32 point_size );

    /// \brief Set a new text string for drawing; defaults to \0
    ///
    /// NOTE: We render the font drawing surface here
    void setText ( const std::string& text );

    /// \brief Load a new TTF or FON from a file
    ///
    /// Support for the file format is determined
    /// by the SDL_ttf extension
    bool Load ( const std::string& filename, const Color& colorkey,
                int32 font_size = 12, bool use_cache = 0
              );

    void Update ( void );

    /// Draw the set text string to the video surface
    void Draw ( void* video_buffer ) /*const*/;

  private:
    /// Surface where font for drawing is rendered to
    Canvas font_buffer;
    /// Font file data, used by SDL_ttf extension
    std::shared_ptr<TTF_Font> font;
    /// holds contents of text as a string buffer
    std::string text_buffer;

    /// Current text effect set
    enum FontStyle text_style;
    uint8 style_options;

    /// Store the file path so we can change font sizes on the fly
    std::string filename;
};


} // namespace nom

#endif // include guard
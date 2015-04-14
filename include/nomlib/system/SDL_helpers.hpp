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
#ifndef NOMLIB_SDL2_HELPERS_HPP
#define NOMLIB_SDL2_HELPERS_HPP

#include <memory>

#include <SDL.h>

#include "nomlib/config.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/math/Point2.hpp"

// FIXME: The following declaration is necessary in order to avoid a very
// nasty compiling conflict that can happen under Windows anytime the
// wingdi.h header file is included (commonly from windows.h), due to min and
// max macros being declared there. This is why macros are evil.
//
// In this case, it's probably not a bad idea to rename our RGB functions...
// seeing as they are terrible names for a function description, anyhow!
//
// http://support.microsoft.com/kb/143208
// http://stackoverflow.com/questions/5004858/stdmin-gives-error
#if defined( NOM_PLATFORM_WINDOWS )
  #undef RGB
#endif

// Forward declarations
struct _TTF_Font {};

namespace nom {

enum BlendMode
{
  /// no blending
  /// dstRGBA = srcRGBA
  BLEND_MODE_NONE = SDL_BLENDMODE_NONE,

  /// alpha blending
  /// dstRGB = (srcRGB * srcA) + (dstRGB * (1-srcA))
  /// dstA = srcA + (dstA * (1-srcA))
  BLEND_MODE_BLEND = SDL_BLENDMODE_BLEND,

  /// additive blending
  /// dstRGB = (srcRGB * srcA) + dstRGB
  /// dstA = dstA
  BLEND_MODE_ADD = SDL_BLENDMODE_ADD,

  /// color modulate
  /// dstRGB = srcRGB * dstRGB
  /// dstA = dstA
  BLEND_MODE_MOD = SDL_BLENDMODE_MOD
};

/// \brief Convenience definitions for pointer types
///
/// \todo Remove or perhaps better yet, refer to as a light-weight handle,
/// i.e.: nom::WindowHandle?
namespace SDL_WINDOW
{
  typedef std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> UniquePtr;
  typedef SDL_Window* RawPtr;
}

/// \brief Convenience definitions for pointer types
///
/// \todo Remove or perhaps better yet, refer to as a light-weight handle..?
namespace SDL_PIXELFORMAT
{
  typedef SDL_PixelFormat* RawPtr;
}

/// \brief Convenience definitions for pointer types
///
/// \todo Remove or perhaps better yet, refer to as a light-weight handle,
/// i.e.: nom::SurfaceHandle?
namespace SDL_SURFACE
{
  typedef std::unique_ptr<SDL_Surface, void(*) (SDL_Surface*)> UniquePtr;
  typedef std::shared_ptr<SDL_Surface> SharedPtr;
  typedef SDL_Surface* RawPtr;
}

/// \brief Convenience definitions for pointer types
///
/// \todo Remove or perhaps better yet, refer to as a light-weight handle,
/// i.e.: nom::TextureHandle?
namespace SDL_TEXTURE
{
  typedef std::shared_ptr<SDL_Texture> SharedPtr;
  typedef SDL_Texture* RawPtr;
}

/// \brief Convert a SDL_BlendMode enumeration value to a nom::BlendMode
/// enumeration value.
BlendMode blend_mode(SDL_BlendMode mode);

/// \brief Convert a nom::BlendMode enumeration value to a SDL_BlendMode
/// enumeration value.
SDL_BlendMode SDL_blend_mode(BlendMode mode);

/// SDL data structure wrappers for nomlib
///
/// \return SDL_Rect structure composed from a nom::IntRect object
SDL_Rect SDL_RECT ( const IntRect& rectangle );

/// \brief Convert nomlib's internal data type(s) to a SDL_Rect struct
///
/// \remarks Frequently used in data passing between nomlib's SDL2 API calls.
///
/// \return SDL_Rect struct composed from a nom::Point2i & nom::Size2i object.
SDL_Rect SDL_RECT ( const Point2i& pos, const Size2i& size );

/// SDL2 data structure wrappers for nomlib
///
/// \return SDL_Point structure composed from a nom::Point2i object
SDL_Point SDL_POINT ( const Point2i& );

/// SDL2 data structure wrappers for nomlib
///
/// \return A SDL_Color structure composed from a nom::Color object
SDL_Color SDL_COLOR ( const Color4i& color );

/// SDL helper functions for nomlib
///
/// \return RGB components of a pixel represented as a nom::Color object
const Color4i pixel ( uint32 pixel, const SDL_PixelFormat* fmt );

/// SDL helper functions for nomlib
///
/// \return RGBA components of a pixel represented as a nom::Color object
const Color4i alpha_pixel ( uint32 pixel, const SDL_PixelFormat* fmt );

const Color4i pixel ( uint32 pixel, uint32 fmt );
const Color4i alpha_pixel ( uint32 pixel, uint32 fmt );

/// SDL helper functions for nomlib
///
/// Obtain a color as an integer, respective to the video surface pixel format
/// (color bit per pixel).
///
/// \return RGB components as an unsigned integer.
uint32 RGB ( const Color4i& color, const SDL_PixelFormat* fmt );

/// SDL2 helper functions for nomlib
///
/// Obtain a color as an integer, respective to the video surface pixel format
/// enum, a new SDL2 structure
///
/// \return RGBA components as an unsigned integer.
uint32 RGB ( const Color4i& color, uint32 fmt );

/// SDL helper functions for nomlib
///
/// Obtain a color as an integer, respective to the video surface pixel format
/// (color bit per pixel).
///
/// \return RGBA components as an unsigned integer.
uint32 RGBA ( const Color4i& color, const SDL_PixelFormat* fmt );

/// SDL2 helper functions for nomlib
///
/// Obtain a color as an integer, respective to the video surface pixel format
/// enum, a new SDL2 structure
///
/// \return RGBA components as an unsigned integer.
uint32 RGBA ( const Color4i& color, uint32 fmt );

/// SDL2 helper function
///
/// Wrapper for SDL_GetHint
///
/// \param name   Hint to query
///
/// \return Value of the queried name, or a null-terminated string if not set
std::string hint(const std::string& name);

/// SDL2 helper function
///
/// Wrapper for SDL_SetHint
///
/// \param name   Name of hint to set
/// \param value  New value to set
///
/// \remarks It is best to set hints **before** the first call to nom::init has
/// been made, as certain hints, i.e.: SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES
/// must be called before video initialization, whereas others depend on a valid
/// renderer, etc.
bool set_hint(const std::string& name, const std::string& value);

const std::string PIXEL_FORMAT_NAME ( uint32 format );

/// Save a screen shot as a Portable Network Graphics (PNG) file.
///
/// \param  buffer          Input SDL_Surface video surface to save
/// \param  filename        Output filename path
///
/// \note SDL2_image appears to be saving PNG files with ARGB pixel
/// ordering; SDL_PIXELFORMAT_ARGB8888
bool save_png ( SDL_SURFACE::RawPtr, const std::string& filename );

/// \brief Query for the platform's support of a rendering driver.
///
/// \remarks The driver name should be one of: directx, opengl, software
///
/// \returns The index of the requested rendering driver on success, or
/// negative one (-1) on failure.
int available_render_driver(const std::string& driver);

namespace priv {

/// Custom deleter for SDL_Window structures
void FreeWindow ( SDL_Window* );

/// Custom deleter for SDL_Renderer structures
void FreeRenderTarget ( SDL_Renderer* );

/// Custom deleter for SDL_Texture structures
void FreeTexture ( SDL_Texture* );

/// Custom deleter for SDL_Surface structures
void FreeSurface ( SDL_Surface* );

/// Custom deleter for TTF_Font* structures
void TTF_FreeFont ( _TTF_Font* );

} // namespace priv
} // namespace nom

/// \brief Output operator overload for SDL_Point (debugging aid)
std::ostream& operator <<( std::ostream& os, const SDL_Point& pos );

/// \brief Output operator overload for SDL_Rect (debugging aid)
std::ostream& operator <<( std::ostream& os, const SDL_Rect& rect );

/// \brief Output operator overload for SDL_Color (debugging aid)
std::ostream& operator <<( std::ostream& os, const SDL_Color& color );

#endif // include guard defined

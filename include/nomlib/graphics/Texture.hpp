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
#ifndef NOMLIB_SDL2_TEXTURE_HEADERS
#define NOMLIB_SDL2_TEXTURE_HEADERS

#include <iostream>
#include <string>
#include <memory>

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/system/SDL_helpers.hpp"
#include "nomlib/math/helpers.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/system/ObjectCache.hpp"
#include "nomlib/system/SDL_helpers.hpp"
#include "nomlib/graphics/Image.hpp"
#include "nomlib/graphics/Window.hpp"

/// \todo Rename NOM_DEBUG_SDL_TEXTURE to NOM_DEBUG_SDL2_TEXTURE
//#define NOM_DEBUG_SDL_TEXTURE
#define NOM_DEBUG_SDL2_RESIZE

namespace nom {

/// Available rescaling algorithms
/// \todo SDL2 port
enum ResizeAlgorithm
{
  None = 0, // No resizing is applied
  scale2x,
  scale3x, // Reserved for future implementation
  scale4x, // Reserved for future implementation
  hq2x,
  hq3x,
  hq4x,
  Stretch
};

class Texture
{
  public:
    typedef std::shared_ptr<Texture> SharedPtr;

    /// Default constructor
    Texture ( void );

    /// Lazy destructor
    ~Texture ( void );

    /// Copy constructor
    Texture ( const Texture& other );

    /// Initialize object with SDL1 video surface
    ///
    /// \note The new texture created will be of type 'SDL_TEXTUREACCESS_STATIC'
    bool initialize ( SDL_SURFACE::RawPtr video_buffer );

    /// Initialize an object with specified parameters
    ///
    /// \param width    Width -- in pixels -- of the new texture
    ///
    /// \param height   Height -- in pixels -- of the new texture
    ///
    /// \param format   A pixel format; one of the enumerated values in
    ///                 SDL_PixelFormatEnum
    ///
    /// \param flags    Texture access type; one of the enumerated values in
    ///                 SDL_TextureAccess
    bool initialize ( int32 width, int32 height, uint32 format, uint32 flags );

    /// Return a std::shared_ptr copy of this instance
    /// \todo Test me out!
    Texture::SharedPtr clone ( void ) const;

    /// Copy assignment operator
    Texture& operator = ( const Texture& other );

    const Point2i& position ( void ) const;

    const Coords& bounds ( void ) const;

    /// Get the video memory surface of the Texture object
    SDL_TEXTURE::RawPtr texture ( void ) const;

    /// Is this object initialized -- not nullptr?
    bool valid ( void ) const;

    /// Query texture access type
    ///
    /// \return SDL_TextureAccess enum type
    int access ( void ) const;

    /// Reset the video surface object with another video surface object.
    /// \todo Relocate me to a surface class
    ///void setTexture ( const Texture& surface );

    void set_position ( const Point2i& pos );

    void set_bounds ( const Coords& clip );

    /// Obtain width, in pixels, of texture
    ///
    /// \return -1 on error; non-zero width on success
    int32 width ( void ) const;

    /// Obtain height, in pixels, of texture
    ///
    /// \return -1 on error; non-zero height on success
    int32 height ( void ) const;

    int pitch ( void ) const;

    void* pixels ( void ) const;

    uint8 bytes_per_pixel ( void ) const;

    uint32 pixel_format ( void ) const;

    /// Calculate this object's bits per pixel (color depth)
    ///
    /// \return Unsigned 8-bit integer (8, 16, 24 or 32 bits per pixel); zero (0)
    /// on error (unknown color depth).
    uint8 bits_per_pixel ( void ) const;

    /// Obtain the blending mode used for texture copies
    const SDL_BlendMode blend_mode ( void ) const;

    /// Query memory lock status
    bool locked ( void ) const;

    /// Obtain the set color key for this image
    ///
    /// \return   Returns non-negative nom::Color on success;
    ///           nom::Color::null on failure
    const Color4u& colorkey ( void ) const;

    const uint8 alpha ( void ) const;

    /// Lock a portion of the texture for **write-only** pixel access.
    ///
    /// Texture must be created with SDL_TEXTUREACCESS_STREAMING.
    bool lock ( void );

    /// Lock a portion of this texture for write-only pixel access.
    ///
    /// Texture must be created with SDL_TEXTUREACCESS_STREAMING.
    ///
    /// \param lock_coords  The area encompassing the area to lock for write
    ///                     access.
    bool lock ( const Coords& lock_coords );

    /// Unlock this texture, thereby uploading any applicable changes
    /// to video memory.
    void unlock ( void );

    /// Load an image into memory from a file
    ///
    /// \param filename         File path you wish to load into memory as a
    ///                         valid bitmap / texture
    ///                         (think: supported image file types).
    ///
    /// \param flags            [...TO BE WRITTEN...]
    ///
    /// \param use_cache        Whether or not to use an internal object cache
    ///                         feature of nomlib. Defaults to off.
    ///
    /// \todo Test/Research Texture caching -- nom::ObjectCache worked
    /// beautifully with SDL_Surface, is the same true of SDL_Texture?
    ///
    /// \todo merge 'use_cache' in with 'flags'
    bool load (
                const std::string& filename, uint32 flags,
                bool use_cache = false
              );

    /// Upload texture copy with new pixel data
    ///
    /// \todo Use update_area argument
    bool update ( const void* pixels, uint16 pitch, const Coords& update_area );

    /// Draw a nom::Texture to a SDL_Renderer target
    ///
    /// \param  SDL_Renderer
    ///
    void draw ( SDL_RENDERER::RawPtr target ) const;

    /// Draw a nom::Texture to a nom::Window target
    ///
    /// \param  nom::Window
    ///
    /// \note This is an alias for nom::Texture::draw ( SDL_Renderer* )
    void draw ( const Window& target ) const;

    /// Draw a rotated nom::Texture to a rendering target
    ///
    /// \param  target  Pointer to SDL_Renderer context
    /// \param  angle   Rotation angle in degrees
    ///
    /// \todo Implement pivot point & make use of SDL_RendererFlip enum
    void draw ( SDL_RENDERER::RawPtr target, const double angle ) const;

    /// Draw a rotated nom::Texture on a nom::Window
    ///
    /// \param  target  Reference to an active nom::Window
    /// \param  angle   Rotation angle in degrees
    void draw ( const Window& target, const double angle ) const;

    bool set_alpha ( uint8 opacity );

    /// Pixel reading -- supports 8-bit, 15/16-bit, 24-bit & 32-bit color modes
    ///
    /// Returns -1 on error
    ///
    /// You are responsible for locking & unlocking of the Texture before-hand
    ///
    /// \todo Test 8-bit, 15/16-bit & 24-bit pixels
    uint32 pixel ( int32 x, int32 y );

    /// Resize the video surface with the chosen rescaling algorithm.
    ///
    /// See the ResizeAlgorithm enum for available rescaling algorithms
    ///
    /// \todo SDL2 port
    bool resize ( enum ResizeAlgorithm scaling_algorithm );

    /// Return the correct scaling factor of the chosen algorithm
    ///
    /// \todo SDL2 port
    int32 getResizeScaleFactor ( enum ResizeAlgorithm scaling_algorithm );

    /// Set a new blending mode for this texture
    bool set_blend_mode ( const SDL_BlendMode blend );

    /// Set a new color key
    ///
    /// \param colorkey     nom::Color object representing the RGB pixel to be
    ///                     marked transparent.
    ///
    /// \todo ERR check on locking call, etc.
    ///
    /// (This method requires locking the texture; use wisely!).
    bool set_colorkey ( const Color4u& colorkey );

  private:
    SDL_TEXTURE::SharedPtr texture_;

    /// Texture's pixels; these are only available when a Texture is locked and
    /// is intended for writing to.
    void* pixels_;

    /// Texture's pixel pitch; these are only available when a Texture is locked.
    int pitch_;

    /// Holds surface position (X, Y)
    Point2i position_;

    /// Holds surface offsets (clipping area: X, Y, width & height)
    Coords bounds_;

    /// Cached upon use of the set_colorkey method for use by external classes
    Color4u colorkey_;
};


} // namespace nom

#endif // include guard defined

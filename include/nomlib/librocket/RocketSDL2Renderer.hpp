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
#ifndef NOMLIB_LIBROCKET_SDL2_RENDERER_INTERFACE_HPP
#define NOMLIB_LIBROCKET_SDL2_RENDERER_INTERFACE_HPP

// #include <memory>

#include "nomlib/config.hpp"

#include <Rocket/Core/Core.h>
#include <Rocket/Core/RenderInterface.h>

#include <SDL.h>

#include <OpenGL/glu.h>

#if !(SDL_VIDEO_RENDER_OGL)
  #error "Only the opengl sdl backend is supported. To add support for others, see http://mdqinc.com/blog/2013/01/integrating-librocket-with-sdl-2/"
#endif

namespace nom {

// Forward declarations
class RenderWindow;

/// \see https://wiki.libsdl.org/SDL_GL_BindTexture
class RocketSDL2Renderer: public Rocket::Core::RenderInterface
{
  public:
    /// \brief Initialize OpenGL with the necessary settings for libRocket and
    /// SDL2 to be all friendly.
    ///
    /// \returns Always boolean TRUE; the return value is not used at this time.
    ///
    /// \param width  The width of the clipping plane (orthographic matrix).
    /// \param height The height of the clipping plane (orthographic matrix).
    ///
    /// \note This method should be called before any rendering is done.
    ///
    /// \remarks This is not part of the "official" interface, and is provided
    /// purely as a convenience method.
    ///
    /// \todo Consider making a call to htis method upon construction? (Probably
    /// not a wise idea...)
    static bool gl_init( int width, int height );

    RocketSDL2Renderer( RenderWindow* window );

    /// Called by Rocket when it wants to render geometry that it does not wish to optimise.
    virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation);

    /// Called by Rocket when it wants to enable or disable scissoring to clip content.
    virtual void EnableScissorRegion(bool enable);
    /// Called by Rocket when it wants to change the scissor region.
    virtual void SetScissorRegion(int x, int y, int width, int height);

    /// Called by Rocket when a texture is required by the library.
    virtual bool LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source);
    /// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
    virtual bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions);
    /// Called by Rocket when a loaded texture is no longer required.
    virtual void ReleaseTexture(Rocket::Core::TextureHandle texture_handle);

    /// \brief nomlib interface bridge between SDL2 and libRocket
    ///
    /// \remarks The interface does **not** own the pointer.
    RenderWindow* window_;
};

} // namespace nom

#endif // include guard defined

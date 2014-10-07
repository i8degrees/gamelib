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
#include "nomlib/librocket/RocketSDL2RenderInterface.hpp"

// Private headers (third-party
#include <SDL.h>

#if defined( NOM_PLATFORM_OSX )
  #include <OpenGL/gl.h>
#else
  #include <glew.h>
#endif

#if !(SDL_VIDEO_RENDER_OGL)
  #error "Only the opengl sdl backend is supported. To add support for others, see http://mdqinc.com/blog/2013/01/integrating-librocket-with-sdl-2/"
#endif

// Forward declarations
#include "nomlib/graphics/RenderWindow.hpp"

// Private headers
#include "nomlib/graphics/Image.hpp"
#include "nomlib/graphics/Texture.hpp"

namespace nom {

// static
bool RocketSDL2RenderInterface::gl_init( int width, int height )
{
  // We get lucky on OS X ... we have up-to-date OpenGL profile to work with,
  // but otherwise... we're probably stuck with the OpenGL v1.1 API, which does
  // not include support for the glUseProgramObjectARB function, used in
  // ::RenderGeometry (introduced in OpenGL v2.0 API).
  //
  // TODO: Does SDL2 or libRocket require shaders? I don't really know! It might
  // be wise to add a GLEW check for the necessary GL extension below ...
  #if ! defined( NOM_PLATFORM_OSX )
    GLenum err = glewInit();

    if( err != GLEW_OK )
    {
      NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, glewGetErrorString(err) );
      // We'd get white, blocky textures if we were to continue past this
      // point without the glUseProgramObjectARB call...
      return false;
    }
  #endif

  // Initialize OpenGL for SDL2 + libRocket play along
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  glMatrixMode( GL_PROJECTION | GL_MODELVIEW );
  glLoadIdentity();
  glOrtho( 0, width, height, 0, 0, 1 );

  return true;
}

RocketSDL2RenderInterface::RocketSDL2RenderInterface( RenderWindow* window )
{
  this->window_ = window;
}

RocketSDL2RenderInterface::~RocketSDL2RenderInterface()
{
}

void RocketSDL2RenderInterface::Release()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  delete this;
}

void RocketSDL2RenderInterface::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, const Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation)
{
  // SDL uses shaders that we need to disable here
  glUseProgramObjectARB(0);
  glPushMatrix();
  glTranslatef(translation.x, translation.y, 0);

  std::vector<Rocket::Core::Vector2f> Positions(num_vertices);
  std::vector<Rocket::Core::Colourb> Colors(num_vertices);
  std::vector<Rocket::Core::Vector2f> TexCoords(num_vertices);
  float texw, texh;

  SDL_Texture* sdl_texture = NULL;
  if(texture)
  {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    sdl_texture = (SDL_Texture *) texture;
    SDL_GL_BindTexture(sdl_texture, &texw, &texh);
  }

  for( int i = 0; i < num_vertices; i++ )
  {
    Positions[i] = vertices[i].position;
    Colors[i] = vertices[i].colour;
    if( sdl_texture )
    {
      TexCoords[i].x = vertices[i].tex_coord.x * texw;
      TexCoords[i].y = vertices[i].tex_coord.y * texh;
    }
    else
    {
      TexCoords[i] = vertices[i].tex_coord;
    }
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, &Positions[0]);
  glColorPointer(4, GL_UNSIGNED_BYTE, 0, &Colors[0]);
  glTexCoordPointer(2, GL_FLOAT, 0, &TexCoords[0]);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

  if (sdl_texture)
  {
    SDL_GL_UnbindTexture(sdl_texture);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  glColor4f(1.0, 1.0, 1.0, 1.0);
  glPopMatrix();

  /* Reset blending and draw a fake point just outside the screen to let SDL know that it needs to reset its state in case it wants to render a texture */
  glDisable(GL_BLEND);
  SDL_SetRenderDrawBlendMode(this->window_->renderer(), SDL_BLENDMODE_NONE);
  SDL_RenderDrawPoint(this->window_->renderer(), -1, -1);

  // Reset the renderer's drawing color; this is necessary because otherwise
  // the GL color call above this statement overwrites any drawing colors set
  // by nomlib's SDL2 rendering subsystem.
  //
  // I don't really know what I'm doing here! I just know that it appears to
  // work in the instance I'm working in (custom libRocket decorator)...
  if( this->window_->set_color( Color4i::Blue ) == false )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
  }

  if( nom::hint( "NOM_LIBROCKET_EMULATE_SDL2_LOGICAL_VIEWPORT" ) == "1" )
  {
    // TODO: This is prototype / proof of concept code for emulating SDL2's
    // SDL_RenderSetLogicalSize function. It works, but needs further improvement,
    // such as being called only when necessary, deal with aspect ratio issues,
    // updating mouse coordinates with the faked ones, etc.
    //
    // See also: shell examples (i.e.: ShellRenderInterfaceOpenGL.cpp).
    // http://gamedev.stackexchange.com/questions/40704/what-is-the-purpose-of-glscissor
    //
    if( this->window_->fullscreen() )
    {
      SDL_Rect dims;
      SDL_RenderGetViewport( this->window_->renderer(), &dims );

      glMatrixMode( GL_PROJECTION | GL_MODELVIEW );
      glLoadIdentity();

      // TODO: Use calculations from SDL_render.c:UpdateLogicalSize.
      glOrtho( 0, dims.w / 2, dims.h / 2, 0, 0, 1.0 );
    }
    else
    {
      SDL_Rect dims;
      SDL_RenderGetViewport( this->window_->renderer(), &dims );

      glMatrixMode( GL_PROJECTION | GL_MODELVIEW );
      glLoadIdentity();
      // glOrtho( 0, this->window_->size().w, this->window_->size().h, 0, 0, 1.0 );
      glOrtho( 0, dims.w, dims.h, 0, 0, 1.0 );
    }
  }
}

void RocketSDL2RenderInterface::EnableScissorRegion(bool enable)
{
  if(enable)
  {
    glEnable(GL_SCISSOR_TEST);
  }
  else
  {
    glDisable(GL_SCISSOR_TEST);
  }
}

void RocketSDL2RenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
  if( nom::hint( "NOM_LIBROCKET_EMULATE_SDL2_LOGICAL_VIEWPORT" ) == "0" )
  {
    Size2i window = this->window_->size();
    glScissor(x, window.w - (y + height), width, height);
  }
  else
  {
    // This helps correct the clipping that otherwise would cut out the contents
    // of the visual debugger.
    //
    // Emulated from the source at SDL_render_gl.c:GL_UpdateViewport
    //
    // TODO: Verify whether or not we need to adjust our calculations to account
    // for Render to Texture -- it appears as though we do need to.
    IntRect viewport;
    Size2i output;

    viewport = this->window_->viewport();
    output = this->window_->output_size();

    // TODO: Adjust to account for scissors; note how the visual debugger
    // window is not clipped when emulation is turned on
    glScissor(  viewport.x, (output.h - viewport.y - y - viewport.h),
                viewport.w, viewport.h );

    // (Debug info assumes window size of 684x580 so we see the entire debugger)
    // NOM_DUMP(viewport);  // viewport : 0, 0, 684,580
    // NOM_DUMP(output);    // output : 684,580
    // Default pos of debugger window
    // NOM_DUMP(x);          // 416
    // NOM_DUMP(y);          // 74
    // NOM_DUMP(width);      // 246
    // NOM_DUMP(height);     // 500
    // (output.h - y - viewport.h) : -74 (debugger without issue)
    // (output.h - y - viewport.h) : -31 (debugger with issue)
    // NOM_DUMP( (output.h - y - viewport.h) );
  }
}

bool RocketSDL2RenderInterface::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
  Rocket::Core::FileInterface* file_interface = Rocket::Core::GetFileInterface();
  Rocket::Core::FileHandle file_handle = file_interface->Open(source);

  if( !file_handle )
  {
    NOM_LOG_ERR( NOM_LOG_CATEGORY_GUI, "Could not obtain file handle for source:", source.CString() );
    return false;
  }

  file_interface->Seek(file_handle, 0, SEEK_END);
  nom::size_type buffer_size = file_interface->Tell(file_handle);
  file_interface->Seek(file_handle, 0, SEEK_SET);

  char* buffer = new char[buffer_size];
  file_interface->Read(buffer, buffer_size, file_handle);
  file_interface->Close(file_handle);

  nom::size_type i;
  for(i = source.Length() - 1; i > 0; i--)
  {
    if(source[i] == '.')
    {
      break;
    }
  }

  Rocket::Core::String extension = source.Substring(i+1, source.Length()-i);

  Image surface;
  if( surface.load_memory( buffer, buffer_size, extension.CString() ) == true)
  {
    // ::ReleaseTexture is responsible for freeing this pointer
    Texture* texture = new Texture();
    if( texture->create( surface ) == true )
    {
      texture_handle = (Rocket::Core::TextureHandle) texture->texture();

      texture_dimensions =
        Rocket::Core::Vector2i(surface.width(), surface.height() );
    }
    else
    {
      NOM_LOG_ERR( NOM_LOG_CATEGORY_GUI, "Could not create texture handle from image source." );
      return false;
    }

    return true;
  }

  NOM_LOG_ERR( NOM_LOG_CATEGORY_GUI, "Could not create texture handle." );
  return false;
}

bool RocketSDL2RenderInterface::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 rmask = 0xff000000;
    Uint32 gmask = 0x00ff0000;
    Uint32 bmask = 0x0000ff00;
    Uint32 amask = 0x000000ff;
  #else
    Uint32 rmask = 0x000000ff;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x00ff0000;
    Uint32 amask = 0xff000000;
  #endif

  Image surface;
  bool ret;

  // ::ReleaseTexture is responsible for freeing this pointer
  Texture* texture = new Texture();

  ret = surface.initialize(
                            // pixels
                            (void*) source,
                            // width
                            source_dimensions.x,
                            // height
                            source_dimensions.y,
                            // bits per pixel
                            32,
                            // pitch
                            source_dimensions.x * 4,
                            rmask, gmask, bmask, amask );

  if( ret )
  {
    if( texture->create(surface) == false )
    {
      NOM_LOG_ERR( NOM_LOG_CATEGORY_GUI, "Could not generate texture from pixel data." );
      return false;
    }

    SDL_SetTextureBlendMode( texture->texture(), SDL_BLENDMODE_BLEND );
    texture_handle = (Rocket::Core::TextureHandle) texture->texture();

    return true;
  }

  NOM_LOG_ERR( NOM_LOG_CATEGORY_GUI, "Could not generate texture." );
  return false;
}

void RocketSDL2RenderInterface::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
  priv::FreeTexture( (SDL_Texture*)texture_handle );
}

} // namespace nom

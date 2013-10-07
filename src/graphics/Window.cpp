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
#include "nomlib/graphics/Window.hpp"
//#include "nomlib/graphics/Renderer.hpp"

namespace nom {

Window::Window ( void ) : window_
    { Window::UniquePtr ( nullptr, priv::FreeWindow ) },
    window_id_ ( -1 ), window_display_id_ ( - 1 ),
    enabled_ ( false )
{
NOM_LOG_TRACE ( NOM );

  if ( SDL_Init ( SDL_INIT_VIDEO ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
  }

  atexit ( SDL_Quit );
}

Window::~Window ( void )
{
NOM_LOG_TRACE ( NOM );
}

bool Window::create (
                      const std::string& window_title, int32 width, int32 height,
                      uint32 window_flags, uint32 context_flags
                    )
{
  this->window_.reset ( SDL_CreateWindow  (
                                            window_title.c_str(),
                                            SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED,
                                            width,
                                            height,
                                            window_flags
                                          )
                      );

  if ( this->window_valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not create SDL window." );
    return false;
  }

  this->initialize ( this->window(), -1, context_flags );
  if ( this->renderer_valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not create SDL renderer." );
    return false;
  }

  // Track our unique identifiers for our brand spanking new window!
  this->window_id_ = this->window_id();
  this->window_display_id_ = this->window_display_id();
  this->enabled_ = true;

  // Try to ensure that we have no leftover artifacts by clearing and filling
  // window with a solid black paint bucket fill.
  this->fill ( Color::Black );

  return true;
}

SDL_Window* Window::window ( void ) const
{
  return this->window_.get();
}

bool Window::window_valid ( void ) const
{
  if ( this->window() != nullptr )
  {
    return true;
  }
  else
  {
    return false;
  }
}

Point2i Window::position ( void ) const
{
  Point2i pos;

  SDL_GetWindowPosition ( this->window(), &pos.x, &pos.y );

  return pos;
}
/*
const uint8 Window::getDisplayColorBits ( void ) const
{
  SDL_Surface* screen = this->window();

  // We prevent a segmentation fault here by providing a means of accessing the
  // video modes without already having initialized the video display via
  // SDL_SetVideoMode.
  if ( screen == nullptr )
  {
    //const SDL_VideoInfo* bpp = SDL_GetVideoInfo();
    //return bpp->vfmt->BitsPerPixel;
  }

  return screen->format->BitsPerPixel;
  return 0;
}
*/

uint32 Window::window_flags ( void ) const
{
  return SDL_GetWindowFlags ( this->window() );
}

/*
uint16 Window::getDisplayPitch ( void ) const
{
  //return SDL_GetVideoSurface()->pitch;
    return 0;
}
*/
/*
void* Window::getDisplayPixels ( void ) const
{
  //return SDL_GetVideoSurface()->pixels;
    return 0;
}
*/

uint32 Window::pixel_format ( void ) const
{
  return SDL_GetWindowPixelFormat ( this->window() );
}

const Coords Window::display_bounds ( void ) const
{
  SDL_Rect display_bounds;
  Coords bounds;

  if ( SDL_GetDisplayBounds ( this->window_display_id(), &display_bounds ) != 0 )
  {
NOM_LOG_ERR ( NOM, "Could not obtain Window index " + std::to_string ( this->window_display_id() ) + "display bounds." );
NOM_LOG_ERR ( NOM, std::string ( SDL_GetError() ) );
    return Coords::null;
  }

  bounds.x = display_bounds.x;
  bounds.y = display_bounds.y;
  bounds.w = display_bounds.w;
  bounds.h = display_bounds.h;

  return bounds;
}

VideoModeList Window::getVideoModes ( void ) const
{
/*
  VideoModeList modes;
  SDL_Rect** mode;

  mode = SDL_ListModes ( nullptr, SDL_FULLSCREEN );

  if ( mode == nullptr )
  {
NOM_LOG_INFO ( NOM, "Any video mode is supported." ); // FIXME?
    return modes;
  }
  else if ( mode == ( SDL_Rect**) - 1 )
  {
NOM_LOG_INFO ( NOM, "No video modes are supported." );
    return modes;
  }
  else
  {
    for ( int32 idx = 0; mode[idx]; idx++ )
    {
      modes.push_back ( VideoMode ( mode[idx]->w, mode[idx]->h, this->getDisplayColorBits() ) );
    }

    std::sort ( modes.begin(), modes.end(), std::greater<VideoMode>()  );
  }
  return modes;
*/
    return VideoModeList();
}
/*
bool Window::getCanvasLock ( void ) const
{
  //return this->window()->locked;
    return false;
}
*/
/*
bool Window::mustLock ( void ) const
{
  if ( SDL_MUSTLOCK ( this->window() ) )
  {
    return true;
  }
  else
  {
    return false;
  }
    return false;
}
*/
/*
bool Window::lock ( void ) const
{
  if ( this->mustLock() == true )
  {
    if ( SDL_LockSurface ( this->window() ) == -1 )
    {
NOM_LOG_ERR ( NOM, "Could not lock video surface memory." );
      return false;
    }
  }
  return true;
}
*/
/*
void Window::unlock ( void ) const
{
  SDL_UnlockSurface ( this->window() );
}
*/
bool Window::flip ( void ) const
{
  if ( SDL_UpdateWindowSurface ( this->window() ) != 0 )
  {
NOM_LOG_ERR ( NOM, "Could not update window surface: " + std::string ( SDL_GetError() ) );
    return false;
  }
  return true;
}

bool Window::fullscreen ( uint32 flags )
{
  if ( SDL_SetWindowFullscreen ( this->window(), flags ) != 0 )
  {
    NOM_LOG_ERR ( NOM, "Could not toggle SDL fullscreen mode." );
    return false;
  }

  return true;
}

const std::string Window::window_title ( void ) const
{
  std::string title = SDL_GetWindowTitle ( this->window() );

  return title;
}

void Window::set_window_title ( const std::string& title )
{
  SDL_SetWindowTitle ( this->window(), title.c_str() );
}

bool Window::set_window_icon ( const std::string& filename )
{
  Image icon; // holds our image in memory during transfer

// FIXME
// (Windows does not like using IMG_Load (SDL2_image extension) for some
// reason, which limits us solely to BMP (Windows Bitmap) files, which
// arguably is inconvenient ;-P I think I just need to take another look at
// the SDL documentation to see if this is a known limitation of their icon
// loader on Windows platform.
// Jeffrey Carpenter <jeffrey.carp@gmail.com> @ 2013-10-01
#if defined ( NOM_PLATFORM_WINDOWS ) // Use SDL's built-in BMP loader
  if ( icon.load_bmp ( filename ) == false ) return false;
#else // Use SDL2_image for additional image types
  if ( icon.load ( filename ) == false ) return false;
#endif

  if ( icon.valid() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not obtain a valid icon." );
    return false;
  }

  SDL_SetWindowIcon ( this->window(), icon.get() );

  return true;
}

void Window::set_position ( int32 x, int32 y )
{
  SDL_SetWindowPosition ( this->window(), x, y );
}

uint32 Window::window_id ( void ) const
{
  return SDL_GetWindowID ( this->window() );
}

int Window::window_display_id ( void ) const
{
  return SDL_GetWindowDisplayIndex ( this->window() );
}

} // namespace nom

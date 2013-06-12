/******************************************************************************
    SDL_Display.cpp

    SDL Display Class Interface Definition

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Display.hpp"

using namespace nom;

SDL_Display::SDL_Display ( void )
{
  #ifdef DEBUG_SDL_DISPLAY_OBJ
    std::cout << "nom::SDL_Display::SDL_Display(): " << "Hello world!" << std::endl << std::endl;
  #endif

  if ( SDL_Init ( SDL_INIT_VIDEO ) != 0 )
  {
    std::cout << "ERR in SDL_Display::SDL_Display() at SDL_Init(): " << SDL_GetError() << std::endl;
  }
}

SDL_Display::~SDL_Display ( void )
{
  // As per docs, we must not free the publicly available surface, AKA
  // SDL_Surface *screen. This is explicitly stated as a role of the SDL_Quit()
  // function.
  //
  // http://sdl.beuc.net/sdl.wiki/SDL_SetVideoMode

  #ifdef DEBUG_SDL_DISPLAY_OBJ
    std::cout << "nom::SDL::Display::~Display(): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif
}

void SDL_Display::createWindow  ( int32_t display_width, int32_t display_height,
                                  int32_t display_colorbit, uint32_t flags )
{
  void* screen = nullptr; // Better safe than sorry!

  screen = SDL_SetVideoMode ( display_width, display_height,
                              display_colorbit, flags
                            );

  if ( screen == nullptr )
  {
    #ifdef DEBUG_SDL_DISPLAY
      std::cout << "ERR in SDL_Display::SDL_Display(): " << SDL_GetError() << std::endl;
    #endif
  }

  assert ( screen != nullptr );
}

// This is important to return precisely as a SDL_Surface for it changes nullptr
// result potentially
void* SDL_Display::get ( void ) const
{
  return dynamic_cast<SDL_Surface*>( SDL_GetVideoSurface() );
}

int32_t SDL_Display::getDisplayWidth ( void ) const
{
  return SDL_GetVideoSurface()->w;
}

int32_t SDL_Display::getDisplayHeight ( void ) const
{
  return SDL_GetVideoSurface()->h;
}

int32_t SDL_Display::getDisplayColorBits ( void ) const
{
  return SDL_GetVideoSurface()->format->BitsPerPixel;
}

uint32_t SDL_Display::getDisplayFlags ( void ) const
{
  return SDL_GetVideoSurface()->flags;
}

uint16_t SDL_Display::getDisplayPitch ( void ) const
{
  return SDL_GetVideoSurface()->pitch;
}

void* SDL_Display::getDisplayPixels ( void ) const
{
  return SDL_GetVideoSurface()->pixels;
}

void* SDL_Display::getDisplayPixelsFormat ( void ) const
{
  return SDL_GetVideoSurface()->format;
}

const nom::Coords SDL_Display::getDisplayBounds ( void ) const
{
  SDL_Rect clip = SDL_GetVideoSurface()->clip_rect;
  nom::Coords clip_coords ( clip.x, clip.y, clip.w, clip.h );
  return clip_coords;
}

void nom::SDL_Display::Update ( void )
{
  if ( SDL_Flip ( static_cast<SDL_Surface*> ( this->get() ) ) != 0 )
  {
    #ifdef DEBUG_SDL_DISPLAY
      std::cout << "ERR in nom::SDL_Display::Update(): " << SDL_GetError() << std::endl;
    #endif
  }
}

// TODO: test me
// As per libSDL docs, this method call should not be used when the display
// surface is locked
void nom::SDL_Display::Update ( const nom::Coords& coords )
{
  SDL_UpdateRect ( static_cast<SDL_Surface*> ( this->get() ), coords.getX(), coords.getY(), coords.getWidth(), coords.getHeight() );
}

void SDL_Display::toggleFullScreenWindow ( int32_t width, int32_t height )
{
  uint32_t flags = 0; // save our current flags before attempting to switch

  flags = this->getDisplayFlags();

  this->createWindow ( width, height, 0, flags ^ SDL_FULLSCREEN );

  //  If for whatever reason, we cannot toggle fullscreen, try reverting
  //  back to our previous configuration
  if ( this->get() == nullptr )
    this->createWindow ( width, height, 0, flags );
}

// FIXME
const std::string SDL_Display::getWindowTitle ( void ) const
{
  char *window_title;
  SDL_WM_GetCaption ( &window_title, nullptr );
  return std::to_string ( *window_title );
}

// TODO
void* SDL_Display::getWindowIcon ( void ) const
{
  return nullptr;
}

void SDL_Display::setWindowTitle ( const std::string& app_name )
{
  SDL_WM_SetCaption ( app_name.c_str(), nullptr );
}

void SDL_Display::setWindowIcon ( const std::string& app_icon )
{
  nom::SDL_Image image; // holds our image in memory during transfer
  nom::SDL_Canvas icon; // icon canvas to load our icon file into

  if ( this->get() != nullptr )
  {
    std::cout << "ERR in SDL_Display::setWindowIcon(): " << "SDL video subsystem has already been initiated." << std::endl << std::endl;
  }

  if ( image.loadFromFile ( app_icon ) == false )
  {
    #ifdef DEBUG_SDL_CANVAS
      std::cout << "ERR in nom::SDL_Display::setWindowIcon(): " << std::endl << std::endl;
    #endif
  }

  // Sets our canvas with our acquired image surface
  icon.setCanvas ( image.get() );
  icon.setTransparent ( nom::Color ( 0, 0, 0 ), SDL_SRCCOLORKEY ); // FIXME?
  SDL_WM_SetIcon ( static_cast<SDL_Surface*> ( icon.get() ) , nullptr );

  icon.freeCanvas();
}

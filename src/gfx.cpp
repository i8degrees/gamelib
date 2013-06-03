/******************************************************************************
    gfx.cpp

    Graphics Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston

******************************************************************************/
#include "gfx.h"

Gfx::Gfx ( unsigned int sdl_flags, unsigned int img_flags )
{
  #ifdef DEBUG_GFX_OBJ
    std::cout << "Gfx::Gfx(): Hello, world!" << "\n" << std::endl;
  #endif

  this->running = false;

  //#ifdef DEBUG_GFX
  this->showFPS ( true );
  //#endif

  this->setFullScreen ( false );

  if ( SDL_Init ( sdl_flags ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::Gfx() at SDL_Init(): " << SDL_GetError() << std::endl;
    #endif

    exit ( EXIT_FAILURE );
  }

  if ( IMG_Init ( img_flags ) != img_flags )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::Gfx() at IMG_Init(): " << IMG_GetError() << std::endl;
    #endif

    exit ( EXIT_FAILURE );
  }
  this->appTime.Start();
}

Gfx::~Gfx ( void )
{
  // cleanup all of the states
  while ( !this->states.empty() )
    this->states.pop_back();

  this->appTime.Stop();

  // As per docs, we must not free the publicly available surface, AKA
  // SDL_Surface *screen. This is explicitly stated as a role of the SDL_Quit()
  // function.
  //
  // http://sdl.beuc.net/sdl.wiki/SDL_SetVideoMode

  #ifdef DEBUG_GFX_OBJ
    std::cout << "Gfx::~Gfx(): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  IMG_Quit ();

  SDL_Quit ();
}

bool Gfx::SetVideoMode (  unsigned int screen_width,
                          unsigned int screen_height,
                          unsigned int screen_bpp,
                          unsigned int video_flags
                        )
{
  SDL_Surface *screen = NULL;

  screen = SDL_SetVideoMode ( screen_width, screen_height, screen_bpp, video_flags );

  if ( screen == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetVideoMode(): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  return true;
}

SDL_Surface* Gfx::getDisplay ( void )
{
  return SDL_GetVideoSurface();
}

signed int Gfx::getDisplayWidth ( void )
{
  SDL_Surface *screen = NULL;
  screen = SDL_GetVideoSurface();
  return screen->w;
}

signed int Gfx::getDisplayHeight ( void )
{
  SDL_Surface *screen = NULL;
  screen = SDL_GetVideoSurface();
  return screen->h;
}

signed int Gfx::getDisplayColorBits ( void )
{
  SDL_Surface *screen = NULL;

  screen = SDL_GetVideoSurface();

  return screen->format->BitsPerPixel;
}

SDL_PixelFormat* Gfx::getDisplayPixelFormat ( void )
{
  SDL_Surface *screen = NULL;

  screen = SDL_GetVideoSurface();

  return screen->format;
}

unsigned int Gfx::getDisplayFlags ( void )
{
  SDL_Surface *screen = NULL;

  screen = SDL_GetVideoSurface();

  return screen->flags;
}

Uint16 Gfx::getDisplayPitch ( void )
{
  SDL_Surface *screen = NULL;

  screen = SDL_GetVideoSurface();

  return screen->pitch;
}

bool Gfx::setAlpha (  SDL_Surface *video_buffer, unsigned char opacity,
                      unsigned int flags )
{
  #ifdef DEBUG_GFX
    if ( opacity > 255 || opacity < 1 )
      std::cout << "ERR in Gfx::setAlpha(): " << "opacity value is out of bounds." << std::endl;
  #endif

  SDL_SetAlpha ( video_buffer, flags, ( unsigned int ) opacity );

  return true;
}

bool Gfx::setTransparent (  SDL_Surface *video_buffer, nom::Color color,
                            unsigned int flags )
{
  unsigned int transparent_color = 0;

  if ( video_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetSurfaceTransparency(): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  // TODO: Alpha value needs testing
  transparent_color = color.getColorAsInt ( video_buffer->format );

  if ( SDL_SetColorKey ( video_buffer, flags, transparent_color ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetSurfaceTransparency(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

// TODO: Alpha value needs testing
SDL_Surface *Gfx::LoadImage ( std::string filename, nom::Color colorkey, unsigned int flags )
{
  SDL_Surface *temp_buffer = NULL;
  SDL_Surface *video_buffer = NULL;

  temp_buffer = IMG_Load ( filename.c_str() );

  if ( temp_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::LoadImage() at IMG_Load(): " << IMG_GetError() << std::endl;
    #endif

    return NULL;
  }

  setTransparent ( temp_buffer, colorkey, flags );

  if ( colorkey.getAlpha() != -1 )
    video_buffer = SDL_DisplayFormatAlpha ( temp_buffer );
  else
    video_buffer = SDL_DisplayFormat ( temp_buffer );


  SDL_FreeSurface ( temp_buffer );
  temp_buffer = NULL;

  return video_buffer;
}

SDL_Surface *Gfx::LoadImage ( std::string filename, unsigned int flags )
{
  SDL_Surface *temp_buffer = NULL;
  SDL_Surface *video_buffer = NULL;

  temp_buffer = IMG_Load ( filename.c_str() );

  if ( temp_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::LoadImage() at IMG_Load(): " << IMG_GetError() << std::endl;
    #endif

    return NULL;
  }

  video_buffer = SDL_DisplayFormat ( temp_buffer );

  SDL_FreeSurface ( temp_buffer );
  temp_buffer = NULL;

  return video_buffer;
}

bool Gfx::DrawSurface ( SDL_Surface *source_buffer, SDL_Surface *video_buffer,
                        const nom::Coords &coords, const nom::Coords &offsets
                      )

{
  // temporary vars to store our wrapped GCoords
  SDL_Rect blit_coords = coords.getSDL_Rect();
  SDL_Rect blit_offsets = offsets.getSDL_Rect();

  if ( source_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawSurface(): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  if ( SDL_BlitSurface ( source_buffer, &blit_offsets, video_buffer, &blit_coords ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawSurface() at SDL_BlitSurface(): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  return true;
}

bool Gfx::updateDisplay ( void )
{
  SDL_Surface *screen = NULL;

  screen = Gfx::getDisplay();

  if ( SDL_Flip ( screen ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::UpdateScreen(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

bool Gfx::updateSurface ( SDL_Surface *video_buffer )
{
  if ( SDL_Flip ( video_buffer ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::UpdateScreen(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

bool Gfx::drawRect  ( SDL_Surface *video_buffer, const nom::Coords &coords,
                      const nom::Color &color
                    )
{
  SDL_Rect rectangle = coords.getSDL_Rect();
  unsigned int rectangle_color = 0;

  rectangle_color = color.getColorAsInt ( video_buffer->format );

  if ( SDL_FillRect ( video_buffer, &rectangle, rectangle_color ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawRectangle(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

void Gfx::setTitle ( std::string app_name )
{
  SDL_WM_SetCaption ( app_name.c_str(), NULL );
}

// NOTE: *MUST* be called before the first call to SDL_SetVideoMode is made
bool Gfx::setIcon ( std::string app_icon, nom::Color color, unsigned int flags )
{
  SDL_Surface *icon_buffer = NULL;

  if ( Gfx::getDisplay() != NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetWindowIcon(): " << "SDL_SetVideoMode() has already been called." << std::endl;
    #endif
    return false;
  }

  icon_buffer = SDL_LoadBMP ( app_icon.c_str() );

  if ( icon_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetWindowIcon(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  if ( Gfx::setTransparent ( icon_buffer, color, flags ) == false )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetWindowIcon(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  SDL_WM_SetIcon ( icon_buffer, NULL );

  SDL_FreeSurface ( icon_buffer );
  icon_buffer = NULL;

  return true;
}

// 32-bit bpp
unsigned int Gfx::getPixel ( SDL_Surface *video_buffer, unsigned int x, unsigned int y )
{
  //Convert the pixels to 32 bit
  unsigned int *pixels = (unsigned int *) video_buffer->pixels;

  //Get the pixel requested
  return pixels[ ( y * video_buffer->w ) + x ];
}

// 32-bit bpp
void Gfx::setPixel ( SDL_Surface *video_buffer, unsigned int x, unsigned int y, nom::Color color )
{
  unsigned char * pixel = (unsigned char *)video_buffer->pixels;

  pixel += (y * video_buffer->pitch) + (x * sizeof(unsigned int));

  *((unsigned int *)pixel) = color.getColorAsInt ( video_buffer->format );
}

void Gfx::drawLine ( SDL_Surface *video_buffer, float x1, float y1, float x2, float y2, nom::Color color )
{
    // Bresenham's line algorithm
    bool steep = ( fabs ( y2 - y1 ) > fabs ( x2 - x1 ) );

    if ( steep )
    {
        std::swap ( x1, y1 );
        std::swap ( x2, y2 );
    }

    if ( x1 > x2 )
    {
        std::swap ( x1, x2 );
        std::swap ( y1, y2 );
    }

    float dx = x2 - x1;
    float dy = fabs ( y2 - y1 );

    float error = dx / 2.0f;
    int ystep = ( y1 < y2 ) ? 1 : -1;
    int y = (int)y1;

    int maxX = (int)x2;

    for(int x=(int)x1; x<maxX; x++)
    {
        if(steep)
        {
            Gfx::setPixel ( video_buffer, y, x, color );
        }
        else
        {
            Gfx::setPixel ( video_buffer, x, y, color);
        }

        error -= dy;
        if(error < 0)
        {
          y += ystep;
          error += dx;
        }
    }
}

bool Gfx::lockSurface ( SDL_Surface *video_buffer )
{
  if ( SDL_MUSTLOCK ( video_buffer ) )
  {
    SDL_LockSurface ( video_buffer );
  }
  else
    return false;

  return true;
}

bool Gfx::unlockSurface ( SDL_Surface *video_buffer )
{
  if ( SDL_MUSTLOCK ( video_buffer ) )
  {
    SDL_UnlockSurface ( video_buffer );
  }
  else
    return false;

  return true;
}

void Gfx::ChangeState( std::unique_ptr<GameState> state )
{
  // cleanup the current state
  if ( !this->states.empty() )
    this->states.pop_back();

  // store the new state
  this->states.push_back( std::move( state ) );
}

void Gfx::PushState( std::unique_ptr<GameState> state )
{
  // pause current state
  if ( !this->states.empty() )
    this->states.back()->Pause();

  // store the new state
  this->states.push_back( std::move( state ) );
}

void Gfx::PopState ( void )
{
  // cleanup the current state
  if ( !this->states.empty() )
    this->states.pop_back();

  // resume previous state
  if ( !this->states.empty () )
    this->states.back()->Resume();
}

void Gfx::PopStateThenChangeState( std::unique_ptr<GameState> state )
{
  // cleanup the current state
  if ( !this->states.empty() )
    this->states.pop_back();

  if ( !this->states.empty () )
  {
    Gfx::ChangeState( std::move( state ) );
  }
}

void Gfx::HandleInput ( void )
{
  // let the state handle events
  this->states.back()->HandleInput ();
}

void Gfx::Update ( void )
{
  // let the state update the scene
  this->states.back()->Update();
}

void Gfx::Draw( SDL_Surface *video_buffer )
{
  // let the state draw the scene
  this->states.back()->Draw ( video_buffer );
}

bool Gfx::isRunning ( void )
{
  if ( this->running )
    return true;
  else
    return false;
}

void Gfx::Run ( void )
{
  this->running = true;
}

void Gfx::Quit ( void )
{
  this->running = false;
}

unsigned int Gfx::getTicks ( void )
{
  return this->appTime.getTicks();
}

bool Gfx::getShowFPS ( void )
{
  return this->show_fps;
}

void Gfx::showFPS ( bool toggle )
{
  this->show_fps = toggle;
}

// Helper method; toggles showing FPS counter (or not)
void Gfx::toggleFPS ( void )
{
  if ( this->getShowFPS() )
    this->showFPS ( false );
  else
    this->showFPS ( true );
}

bool Gfx::isFullScreen ( void )
{
  if ( this->fullscreen )
    return true;
  else
    return false;
}

void Gfx::setFullScreen ( bool toggle )
{
  this->fullscreen = toggle;
}

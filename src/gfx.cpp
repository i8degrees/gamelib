/******************************************************************************
    gfx.cpp

    Graphics Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston

******************************************************************************/
#include "gfx.h"

using namespace nom;

Gfx::Gfx ( unsigned int img_flags )
{
  #ifdef DEBUG_GFX_OBJ
    std::cout << "Gfx::Gfx(): Hello, world!" << "\n" << std::endl;
  #endif

  this->running = false;

  //#ifdef DEBUG_GFX
  this->showFPS ( true );
  //#endif

  this->setFullScreen ( false );

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

  #ifdef DEBUG_GFX_OBJ
    std::cout << "Gfx::~Gfx(): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  IMG_Quit ();
}

bool Gfx::setAlpha (  void* video_buffer, unsigned char opacity,
                      unsigned int flags )
{
  #ifdef DEBUG_GFX
    if ( opacity > 255 || opacity < 1 )
      std::cout << "ERR in Gfx::setAlpha(): " << "opacity value is out of bounds." << std::endl;
  #endif

  SDL_SetAlpha ( (SDL_Surface*) video_buffer, flags, ( unsigned int ) opacity );

  return true;
}

bool Gfx::setTransparent (  void* video_buffer, const nom::Color& color,
                            unsigned int flags )
{
  SDL_Surface *buffer = (SDL_Surface*) video_buffer; // FIXME

  unsigned int transparent_color = 0;

  if ( buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetSurfaceTransparency(): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  // TODO: Alpha value needs testing
  transparent_color = color.getColorAsInt ( buffer->format );

  if ( SDL_SetColorKey ( buffer, flags, transparent_color ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetSurfaceTransparency(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

///
/// TODO: Alpha value needs testing
///
/// Must be called after video init due to SDL_DisplayFormat call being made
///
void* Gfx::LoadImage ( std::string filename, const nom::Color& colorkey, unsigned int flags )
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

  // Add check in for if SDL_SRCALPHA flag is set
  if ( colorkey.getAlpha() != -1 )
    video_buffer = SDL_DisplayFormatAlpha ( temp_buffer );
  else
    video_buffer = SDL_DisplayFormat ( temp_buffer );

  SDL_FreeSurface ( temp_buffer );
  temp_buffer = NULL;

  return video_buffer;
}

bool Gfx::DrawSurface ( void* source_buffer, void* video_buffer,
                        const nom::Coords &coords, const nom::Coords &offsets
                      )
{
  // temporary vars to store our wrapped GCoords
  SDL_Rect blit_coords = coords.getSDL_Rect();
  SDL_Rect blit_offsets = offsets.getSDL_Rect();

  if ( (SDL_Surface*) source_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawSurface(): NULL surface" << std::endl;
    #endif

    return false;
  }

  // Coords ( -1, -1, -1, -1 ) -- our default, is equivalent to NULL for SDL_BlitSurface it seems
  if ( SDL_BlitSurface ( (SDL_Surface*) source_buffer, &blit_offsets, (SDL_Surface*)video_buffer, &blit_coords ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawSurface() at SDL_BlitSurface(): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  return true;
}

bool Gfx::updateSurface ( void* video_buffer )
{
  if ( SDL_Flip ( (SDL_Surface*) video_buffer ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::UpdateScreen(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

// 32-bit bpp
unsigned int Gfx::getPixel ( void* video_buffer, unsigned int x, unsigned int y )
{
  SDL_Surface* buffer = (SDL_Surface*) video_buffer;

  //Convert the pixels to 32 bit
  unsigned int *pixels = (unsigned int *) buffer->pixels;

  //Get the pixel requested
  return pixels[ ( y * buffer->w ) + x ];
}

// 32-bit bpp
void Gfx::setPixel ( void* video_buffer, unsigned int x, unsigned int y, nom::Color color )
{
  SDL_Surface* buffer = (SDL_Surface*) video_buffer;

  unsigned char * pixel = (unsigned char *)buffer->pixels;

  pixel += (y * buffer->pitch) + (x * sizeof(unsigned int));

  *((unsigned int *)pixel) = color.getColorAsInt ( buffer->format );
}

void Gfx::drawLine ( void* video_buffer, float x1, float y1, float x2, float y2, nom::Color color )
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

bool Gfx::lockSurface ( void* video_buffer )
{
  SDL_Surface* buffer = (SDL_Surface*) video_buffer;

  if ( SDL_MUSTLOCK ( buffer ) )
  {
    SDL_LockSurface ( buffer );
  }
  else
    return false;

  return true;
}

bool Gfx::unlockSurface ( void* video_buffer )
{
  SDL_Surface* buffer = (SDL_Surface*) video_buffer;

  if ( SDL_MUSTLOCK ( buffer ) )
  {
    SDL_UnlockSurface ( buffer );
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

void Gfx::Draw( void* video_buffer )
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

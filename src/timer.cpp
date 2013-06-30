/******************************************************************************
    timer.cpp

    SDL-based Timer Implementation Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "timer.h"

nom::Timer::Timer ( void )
{
  #ifdef DEBUG_TIMER_OBJ
    std::cout << "nom::Timer::Timer (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  if ( SDL_InitSubSystem ( SDL_INIT_TIMER ) == -1 )
  {
    std::cout << "ERR in nom::Timer::Timer() at: " << SDL_GetError() << std::endl;
    return;
  }

  this->started = false;
  this->paused = false;
  this->elapsed_ticks = 0;
  this->paused_ticks = 0;
}

nom::Timer::~Timer ( void )
{
  #ifdef DEBUG_TIMER_OBJ
    std::cout << "nom::Timer::~Timer (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  SDL_QuitSubSystem ( SDL_INIT_TIMER );
}

void nom::Timer::Start ( void )
{
  this->elapsed_ticks = SDL_GetTicks ();
  this->started = true;
  this->paused = false;
}

void nom::Timer::Stop ( void )
{
  this->started = false;
  this->paused = false;
}

void nom::Timer::Pause ( void )
{
  if ( ( this->started == true ) && ( this->paused == false ) )
  {
    this->paused = true;
    this->paused_ticks = SDL_GetTicks() - this->elapsed_ticks;
  }
}

void nom::Timer::Unpause ( void )
{
  if ( this->paused == true )
  {
    this->paused = false;
    this->elapsed_ticks = SDL_GetTicks() - this->paused_ticks;
    this->paused_ticks = 0;
  }
}

unsigned int nom::Timer::getTicks ( void )
{
  if ( this->started == true )
  {
    if ( this->paused == true )
    {
      return this->paused_ticks;
    }
    else
    {
      return SDL_GetTicks() - this->elapsed_ticks;
    }
  }
  return 0;
}

bool nom::Timer::isStarted ( void )
{
  return this->started;
}

bool nom::Timer::isPaused ( void )
{
  return this->paused;
}

void nom::Timer::sleep ( float milliseconds )
{
  SDL_Delay ( milliseconds );
}

uint32_t nom::Timer::seconds ( float seconds ) const
{
  return static_cast<uint32_t> ( seconds * 1000.f );
}

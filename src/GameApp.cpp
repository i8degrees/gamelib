/******************************************************************************
    GameApp.cpp

    Game app interface class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "GameApp.hpp"

using namespace nom;

GameApp::GameApp ( void )
{
  #ifdef DEBUG_GAMEAPP_OBJ
    std::cout << "GameApp::GameApp(): Hello, world!" << std::endl << std::endl;
  #endif

  this->app_state = false;

  //#ifdef DEBUG_GFX
  this->showFPS ( true );
  //#endif

  this->setFullScreen ( false );

  this->appTime.Start();
}

GameApp::~GameApp ( void )
{
  #ifdef DEBUG_GAMEAPP_OBJ
    std::cout << "GameApp::~GameApp(): Goodbye cruel world!" << std::endl << std::endl;
  #endif

  this->setFullScreen ( false );

  this->appTime.Stop();

  this->app_state = false;

  SDL_Quit();
}

bool GameApp::isRunning ( void )
{
  if ( this->app_state )
    return true;
  else
    return false;
}

void GameApp::Running ( void )
{
  this->app_state = true;
}

void GameApp::Quit ( void )
{
  this->app_state = false;
}

uint32_t GameApp::getTicks ( void )
{
  return this->appTime.getTicks();
}

bool GameApp::getShowFPS ( void )
{
  return this->show_fps;
}

void GameApp::showFPS ( bool toggle )
{
  this->show_fps = toggle;
}

// Helper method; toggles showing FPS counter (or not)
void GameApp::toggleFPS ( void )
{
  if ( this->getShowFPS() )
    this->showFPS ( false );
  else
    this->showFPS ( true );
}

bool GameApp::isFullScreen ( void )
{
  if ( this->fullscreen )
    return true;
  else
    return false;
}

void GameApp::setFullScreen ( bool toggle )
{
  this->fullscreen = toggle;
}

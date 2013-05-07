/******************************************************************************
    SDLInput.h

    Input Events Handling

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_SDLINPUT_HEADERS
#define GAMELIB_SDLINPUT_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "gamelib.h"

class SDLInput
{
  public:
    SDLInput ( void );
    virtual ~SDLInput ( void );

    void Input ( void );

    virtual void onExit ( void );
    virtual void onResize ( unsigned int width, unsigned int height ); // TODO
    virtual void onRestore ( void ); // TODO
    virtual void onMinimize ( void ); // TODO
    virtual void onInputFocus ( void ); // TODO
    virtual void onMouseFocus ( void ); // TODO

    virtual void onKeyDown ( SDLKey key, SDLMod mod );
    virtual void onKeyUp ( SDLKey key, SDLMod mod );

    virtual void onMouseMotion ( unsigned int x, unsigned int y );
    virtual void onMouseWheel ( bool up, bool down ); // TODO
    virtual void onMouseLeftButtonDown ( unsigned int x, unsigned int y );
    virtual void onMouseMiddleButtonDown ( unsigned int x, unsigned int y );
    virtual void onMouseRightButtonDown ( unsigned int x, unsigned int y );
    virtual void onMouseLeftButtonUp ( unsigned int x, unsigned int y );
    virtual void onMouseMiddleButtonUp ( unsigned int x, unsigned int y );
    virtual void onMouseRightButtonUp ( unsigned int x, unsigned int y );

    // TODO: joystick

  private:
    //
    SDL_Event input;
};

#endif // GAMELIB_SDLINPUT_HEADERS defined

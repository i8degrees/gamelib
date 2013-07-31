/******************************************************************************

    SDL Input Events Handling

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_INPUT_HEADERS
#define NOMLIB_SDL_INPUT_HEADERS

#include <iostream>
#include <string>

#include <SDL/SDL.h>

#include "nomlib/config.hpp"
#include "nomlib/sys/Logger.hpp"

namespace nom {

/// SDL mapping to individual PS3 axes
///
/// TODO: Needs testing / verification
enum PSXAXES
{
  LEFTX = 0,
  LEFTY = 1,
  RIGHTX = 2,
  RIGHTY = 3
};

/// SDL mapping to individual PS3 controller buttons
enum PSXBUTTON
{
  SELECT            = 0,
  JOYCLICKLEFT      = 1,
  JOYCLICKRIGHT     = 2,
  START             = 3,
  UP                = 4,
  RIGHT             = 5,
  DOWN              = 6,
  LEFT              = 7,
  L2                = 8,
  R2                = 9,
  L1                = 10,
  R1                = 11,
  TRIANGLE          = 12,
  CIRCLE            = 13,
  CROSS             = 14,
  SQUARE            = 15,
  PSBUTTON          = 16
};

// This is an inheritance-only class
class Input
{
  public:
    Input ( void );
    virtual ~Input ( void );

    virtual void HandleInput ( void* event );
  protected:
    virtual void onUserEvent ( uint8_t type, int32_t code, void* data1, void* data2 );
    virtual void onQuit ( void );
    virtual void onResize ( int32_t width, int32_t height );
    virtual void onRestore ( void );
    virtual void onMinimize ( void );
    virtual void onInputFocus ( void );
    virtual void onInputBlur ( void );
    virtual void onMouseFocus ( void );
    virtual void onMouseBlur ( void );
    virtual void onExpose ( void );

    virtual void onKeyDown ( int32_t key, int32_t mod );
    virtual void onKeyUp ( int32_t key, int32_t mod );

    virtual void onMouseMotion ( int32_t x, int32_t y );
    virtual void onMouseWheel ( bool up, bool down );
    virtual void onMouseLeftButtonDown ( int32_t x, int32_t y );
    virtual void onMouseMiddleButtonDown ( int32_t x, int32_t y );
    virtual void onMouseRightButtonDown ( int32_t x, int32_t y );
    virtual void onMouseButtonSixDown ( nom::int32 x, nom::int32 y );
    virtual void onMouseButtonSevenDown ( nom::int32 x, nom::int32 y );

    virtual void onMouseLeftButtonUp ( int32_t x, int32_t y );
    virtual void onMouseMiddleButtonUp ( int32_t x, int32_t y );
    virtual void onMouseRightButtonUp ( int32_t x, int32_t y );
    virtual void onMouseButtonSixUp ( nom::int32 x, nom::int32 y );
    virtual void onMouseButtonSevenUp ( nom::int32 x, nom::int32 y );

    virtual void onJoyButtonDown ( int32_t which, int32_t button );
    virtual void onJoyButtonUp ( int32_t which, int32_t button );
    virtual void onJoyAxis ( int32_t which, int32_t axis, short value );

  private:
    SDL_Joystick* joystick;
};


} // namespace nom

#endif // NOMLIB_SDL_INPUT_HEADERS defined
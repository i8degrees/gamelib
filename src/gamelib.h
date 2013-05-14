/******************************************************************************
    gamelib.h

    SDL-based Game Engine Library

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_HEADERS
#define GAMELIB_HEADERS

#include "SDLInput.h"
#include "OSXFs.h"
#include "gfx.h"
//#include "font.h"
#include "SDL_TFont.h"
#include "sprite.h"
#include "SDL_BitmapFont.h"
#include "SDLWindowFrame.h"
#include "SDLGradient.h"
#include "SDLMessageBox.h"
#include "timer.h"
#include "fps.h" // inherits from Timer
#include "audio.h"

#define DEBUG_COLOR
//#define DEBUG_COLOR_OBJ

#define DEBUG_COORDS
#define DEBUG_COORDS_OBJ

#define DEBUG_AUDIO
#define DEBUG_AUDIO_OBJ

#define DEBUG_SDL_TFONT
#define DEBUG_SDL_TFONT_OBJ

#define DEBUG_BITMAP_FONT
#define DEBUG_BITMAP_FONT_OBJ

#define DEBUG_GFX
#define DEBUG_GFX_OBJ

//#define DEBUG_GRADIENT
//#define DEBUG_GRADIENT_OBJ

#define DEBUG_SPRITE
#define DEBUG_SPRITE_OBJ

#define DEBUG_SDLAPP
#define DEBUG_SDLAPP_OBJ

#define DEBUG_SDLINPUT
#define DEBUG_SDLINPUT_OBJ

#define DEBUG_OSXFS
#define DEBUG_OSXFS_OBJ

#define DEBUG_TIMER
#define DEBUG_TIMER_OBJ

#define DEBUG_TIMER_FPS
#define DEBUG_TIMER_FPS_OBJ

#define DEBUG_WINDOWFRAME
#define DEBUG_WINDOWFRAME_OBJ

#endif // GAMELIB_HEADERS defined

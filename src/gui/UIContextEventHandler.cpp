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
#include "nomlib/gui/UIContextEventHandler.hpp"

// Forward declarations (third-party)
#include <Rocket/Core/Context.h>
#include <Rocket/Debugger/Debugger.h>

// Forward declarations
#include "nomlib/gui/UIContext.hpp"
#include "nomlib/system/Event.hpp"

namespace nom {

UIContextEventHandler::UIContextEventHandler( UIContext* ctx ) :
  ctx_( ctx )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  NOM_ASSERT( this->ctx_->context() != nullptr );
}

UIContextEventHandler::~UIContextEventHandler()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

void UIContextEventHandler::process_event(const nom::Event& ev)
{
  switch( ev.type )
  {
    default: break;

    case nom::Event::WINDOW_EVENT:
    {
      switch(ev.window.event)
      {
        case nom::WindowEvent::SIZE_CHANGED:
        {
          // Update desktop dimensions; this should not be used with SDL2's
          // independent resolution scale feature (logical view-port),
          // as it breaks the absolute positioning coordinates. This is due to
          // the internally calculated aspect ratio that SDL2 does when using
          // the feature upon a size change.
          // this->ctx_->set_size( Size2i( ev.window.data1, ev.window.data2 ) );
        } break;
      }
    } break;

    case nom::Event::MOUSE_MOTION:
    {
      this->ctx_->context()->ProcessMouseMove(  ev.motion.x,
                                                ev.motion.y,
                                                this->translate_key_modifiers(ev) );
    } break;

    case nom::Event::MOUSE_BUTTON_CLICK:
    {
      this->ctx_->context()->ProcessMouseButtonDown(  this->translate_mouse_button(ev),
                                                      this->translate_key_modifiers(ev) );
    } break;

    case nom::Event::MOUSE_BUTTON_RELEASE:
    {
      this->ctx_->context()->ProcessMouseButtonUp(  this->translate_mouse_button(ev),
                                                    this->translate_key_modifiers(ev) );
    } break;

    case nom::Event::MOUSE_WHEEL:
    {
      this->ctx_->context()->ProcessMouseWheel( this->translate_mouse_wheel(ev),
                                                this->translate_key_modifiers(ev) );
    } break;

    case Event::KEY_PRESS:
    {
      // Intercept key input shift-~ for toggling the visual debugger
      if( ev.key.sym == SDLK_BACKQUOTE && ev.key.mod == KMOD_LSHIFT )
      {
        NOM_ASSERT( this->ctx_->valid() == true );
        if( this->ctx_->debugger_enabled() == true )
        {
          Rocket::Debugger::SetVisible( ! Rocket::Debugger::IsVisible() );
          break;
        }
      }

      this->ctx_->context()->ProcessKeyDown(  this->translate_key(ev),
                                              this->translate_key_modifiers(ev) );
    } break;

    case Event::KEY_RELEASE:
    {
      this->ctx_->context()->ProcessKeyUp(  this->translate_key(ev),
                                            this->translate_key_modifiers(ev) );
    } break;

    // TODO: Implement
    case Event::TEXT_INPUT:
    {
      this->ctx_->context()->ProcessTextInput(ev.text.text);
    } break;
  }
}

Rocket::Core::Input::KeyIdentifier
UIContextEventHandler::translate_key( const Event& ev )
{
  using namespace Rocket::Core::Input;

  switch( ev.key.sym )
  {
    case SDLK_UNKNOWN:
      return KI_UNKNOWN;
      break;
    case SDLK_SPACE:
      return KI_SPACE;
      break;
    case SDLK_0:
      return KI_0;
      break;
    case SDLK_1:
      return KI_1;
      break;
    case SDLK_2:
      return KI_2;
      break;
    case SDLK_3:
      return KI_3;
      break;
    case SDLK_4:
      return KI_4;
      break;
    case SDLK_5:
      return KI_5;
      break;
    case SDLK_6:
      return KI_6;
      break;
    case SDLK_7:
      return KI_7;
      break;
    case SDLK_8:
      return KI_8;
      break;
    case SDLK_9:
      return KI_9;
      break;
    case SDLK_a:
      return KI_A;
      break;
    case SDLK_b:
      return KI_B;
      break;
    case SDLK_c:
      return KI_C;
      break;
    case SDLK_d:
      return KI_D;
      break;
    case SDLK_e:
      return KI_E;
      break;
    case SDLK_f:
      return KI_F;
      break;
    case SDLK_g:
      return KI_G;
      break;
    case SDLK_h:
      return KI_H;
      break;
    case SDLK_i:
      return KI_I;
      break;
    case SDLK_j:
      return KI_J;
      break;
    case SDLK_k:
      return KI_K;
      break;
    case SDLK_l:
      return KI_L;
      break;
    case SDLK_m:
      return KI_M;
      break;
    case SDLK_n:
      return KI_N;
      break;
    case SDLK_o:
      return KI_O;
      break;
    case SDLK_p:
      return KI_P;
      break;
    case SDLK_q:
      return KI_Q;
      break;
    case SDLK_r:
      return KI_R;
      break;
    case SDLK_s:
      return KI_S;
      break;
    case SDLK_t:
      return KI_T;
      break;
    case SDLK_u:
      return KI_U;
      break;
    case SDLK_v:
      return KI_V;
      break;
    case SDLK_w:
      return KI_W;
      break;
    case SDLK_x:
      return KI_X;
      break;
    case SDLK_y:
      return KI_Y;
      break;
    case SDLK_z:
      return KI_Z;
      break;
    case SDLK_SEMICOLON:
      return KI_OEM_1;
      break;
    case SDLK_PLUS:
      return KI_OEM_PLUS;
      break;
    case SDLK_COMMA:
      return KI_OEM_COMMA;
      break;
    case SDLK_MINUS:
      return KI_OEM_MINUS;
      break;
    case SDLK_PERIOD:
      return KI_OEM_PERIOD;
      break;
    case SDLK_SLASH:
      return KI_OEM_2;
      break;
    case SDLK_BACKQUOTE:
      return KI_OEM_3;
      break;
    case SDLK_LEFTBRACKET:
      return KI_OEM_4;
      break;
    case SDLK_BACKSLASH:
      return KI_OEM_5;
      break;
    case SDLK_RIGHTBRACKET:
      return KI_OEM_6;
      break;
    case SDLK_QUOTEDBL:
      return KI_OEM_7;
      break;
    case SDLK_KP_0:
      return KI_NUMPAD0;
      break;
    case SDLK_KP_1:
      return KI_NUMPAD1;
      break;
    case SDLK_KP_2:
      return KI_NUMPAD2;
      break;
    case SDLK_KP_3:
      return KI_NUMPAD3;
      break;
    case SDLK_KP_4:
      return KI_NUMPAD4;
      break;
    case SDLK_KP_5:
      return KI_NUMPAD5;
      break;
    case SDLK_KP_6:
      return KI_NUMPAD6;
      break;
    case SDLK_KP_7:
      return KI_NUMPAD7;
      break;
    case SDLK_KP_8:
      return KI_NUMPAD8;
      break;
    case SDLK_KP_9:
      return KI_NUMPAD9;
      break;
    case SDLK_KP_ENTER:
      return KI_NUMPADENTER;
      break;
    case SDLK_KP_MULTIPLY:
      return KI_MULTIPLY;
      break;
    case SDLK_KP_PLUS:
      return KI_ADD;
      break;
    case SDLK_KP_MINUS:
      return KI_SUBTRACT;
      break;
    case SDLK_KP_PERIOD:
      return KI_DECIMAL;
      break;
    case SDLK_KP_DIVIDE:
      return KI_DIVIDE;
      break;
    case SDLK_KP_EQUALS:
      return KI_OEM_NEC_EQUAL;
      break;
    case SDLK_BACKSPACE:
      return KI_BACK;
      break;
    case SDLK_TAB:
      return KI_TAB;
      break;
    case SDLK_CLEAR:
      return KI_CLEAR;
      break;
    case SDLK_RETURN:
      return KI_RETURN;
      break;
    case SDLK_PAUSE:
      return KI_PAUSE;
      break;
    case SDLK_CAPSLOCK:
      return KI_CAPITAL;
      break;
    case SDLK_PAGEUP:
      return KI_PRIOR;
      break;
    case SDLK_PAGEDOWN:
      return KI_NEXT;
      break;
    case SDLK_END:
      return KI_END;
      break;
    case SDLK_HOME:
      return KI_HOME;
      break;
    case SDLK_LEFT:
      return KI_LEFT;
      break;
    case SDLK_UP:
      return KI_UP;
      break;
    case SDLK_RIGHT:
      return KI_RIGHT;
      break;
    case SDLK_DOWN:
      return KI_DOWN;
      break;
    case SDLK_INSERT:
      return KI_INSERT;
      break;
    case SDLK_DELETE:
      return KI_DELETE;
      break;
    case SDLK_HELP:
      return KI_HELP;
      break;
    case SDLK_F1:
      return KI_F1;
      break;
    case SDLK_F2:
      return KI_F2;
      break;
    case SDLK_F3:
      return KI_F3;
      break;
    case SDLK_F4:
      return KI_F4;
      break;
    case SDLK_F5:
      return KI_F5;
      break;
    case SDLK_F6:
      return KI_F6;
      break;
    case SDLK_F7:
      return KI_F7;
      break;
    case SDLK_F8:
      return KI_F8;
      break;
    case SDLK_F9:
      return KI_F9;
      break;
    case SDLK_F10:
      return KI_F10;
      break;
    case SDLK_F11:
      return KI_F11;
      break;
    case SDLK_F12:
      return KI_F12;
      break;
    case SDLK_F13:
      return KI_F13;
      break;
    case SDLK_F14:
      return KI_F14;
      break;
    case SDLK_F15:
      return KI_F15;
      break;
    case SDLK_NUMLOCKCLEAR:
      return KI_NUMLOCK;
      break;
    case SDLK_SCROLLLOCK:
      return KI_SCROLL;
      break;
    case SDLK_LSHIFT:
      return KI_LSHIFT;
      break;
    case SDLK_RSHIFT:
      return KI_RSHIFT;
      break;
    case SDLK_LCTRL:
      return KI_LCONTROL;
      break;
    case SDLK_RCTRL:
      return KI_RCONTROL;
      break;
    case SDLK_LALT:
      return KI_LMENU;
      break;
    case SDLK_RALT:
      return KI_RMENU;
      break;
    case SDLK_LGUI:
      return KI_LMETA;
      break;
    case SDLK_RGUI:
      return KI_RMETA;
      break;
    /*case SDLK_LSUPER:
        return KI_LWIN;
        break;
    case SDLK_RSUPER:
        return KI_RWIN;
        break;*/
    default:
      return KI_UNKNOWN;
      break;
  }
}

int UIContextEventHandler::translate_mouse_button(const Event& ev)
{
  int result = ev.mouse.button;

  switch(ev.mouse.button)
  {
    default:
    {
      NOM_ASSERT_INVALID_PATH();
    } break;

    case nom::MouseButton::LEFT_MOUSE_BUTTON:
    {
      result = 0;
    } break;

    case nom::MouseButton::RIGHT_MOUSE_BUTTON:
    {
      result = 1;
    } break;

    case nom::MouseButton::MIDDLE_MOUSE_BUTTON:
    {
      result = 2;
    } break;

    case nom::MouseButton::X1_MOUSE_BUTTON:
    {
      result = 3;
    } break;

    case nom::MouseButton::X2_MOUSE_BUTTON:
    {
      result = 4;
    } break;
  }

  return result;
}

int UIContextEventHandler::translate_mouse_wheel(const Event& ev)
{
  int result = 0;

  if( ev.wheel.y > 0 ) {
    // Up
    result = -1;
  } else if( ev.wheel.y < 0 ) {
    // Down
    result = 1;
  } else {
    // Invalid
  }

  return result;
}

int UIContextEventHandler::translate_key_modifiers( const Event& ev )
{
  uint16 mod = ev.key.mod;

  int retval = 0;

  if( mod & KMOD_CTRL )
  {
    retval |= Rocket::Core::Input::KM_CTRL;
  }

  if( mod & KMOD_SHIFT )
  {
    retval |= Rocket::Core::Input::KM_SHIFT;
  }

  if( mod & KMOD_ALT )
  {
    retval |= Rocket::Core::Input::KM_ALT;
  }

  return retval;
}

} // namespace nom

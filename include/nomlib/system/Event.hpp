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
#ifndef NOMLIB_SYSTEM_EVENT_HPP
#define NOMLIB_SYSTEM_EVENT_HPP

#include <SDL.h>

#include "nomlib/config.hpp"

namespace nom {

const uint8 MAX_TEXT_INPUT_LENGTH = 255;

class EventCallback;

/// \todo Implement QuitEvent struct
/// \remarks SDL_QUIT; event type 256 (dec).

/// \brief A structure containing information on a window event.
///
/// \remarks SDL_WINDOWEVENT; event type 512 (dec).
struct WindowEvent
{
  /// \brief An enumeration of window events.
  ///
  /// \remarks SDL_WINDOWEVENT_NONE; event type 0 (dec).
  /// \remarks SDL_WINDOWEVENT_SHOWN; event type 1 (dec).
  /// \remarks SDL_WINDOWEVENT_HIDDEN; event type 2 (dec).
  /// \remarks SDL_WINDOWEVENT_EXPOSED; event type 3 (dec).
  /// \remarks SDL_WINDOWEVENT_MOVED; event type 4 (dec).
  /// \remarks SDL_WINDOWEVENT_RESIZED; event type 5 (dec).
  /// \remarks SDL_WINDOWEVENT_SIZE_CHANGED; event type 6 (dec).
  /// \remarks SDL_WINDOWEVENT_MINIMIZED; event type 7 (dec).
  /// \remarks SDL_WINDOWEVENT_MAXIMIZED; event type 8 (dec).
  /// \remarks SDL_WINDOWEVENT_RESTORED; event type 9 (dec).
  /// \remarks SDL_WINDOWEVENT_ENTER; event type 10 (dec).
  /// \remarks SDL_WINDOWEVENT_LEAVE; event type 11 (dec).
  /// \remarks SDL_WINDOWEVENT_FOCUS_GAINED; event type 12 (dec).
  /// \remarks SDL_WINDOWEVENT_FOCUS_LOST; event type 13 (dec).
  /// \remarks SDL_WINDOWEVENT_CLOSE; event type 14 (dec).
  uint8 event;

  /// \brief Event dependent data; typically the X coordinate position or width
  /// data of an event.
  int32 data1;

  /// \brief Event dependent data; typically the Y coordinate position or height
  /// data of an event.
  int32 data2;

  /// \brief The identifier of the window at the moment of the event.
  uint32 window_id;

  void dump( void ) const
  {
    NOM_DUMP( NOM_SCAST( int, event ) );
    NOM_DUMP( data1 );
    NOM_DUMP( data2 );
    NOM_DUMP( window_id );
  }
};

struct KeyboardSymbolCode
{
  // TODO
};

struct KeyboardScanCode
{
  // TODO
};

/// \brief A structure containing information on a keyboard symbol.
struct KeyboardSymbolEvent
{
  /// \brief The event type.
  ///
  /// \remarks Not used; reserved for future use.
  uint32 type;

  /// \brief The physical key code of the key press event.
  ///
  /// \todo Implement using the KeyboardScanCode structure.
  SDL_Scancode scan_code;

  /// \brief Virtual key code of the key press event.
  ///
  /// \todo Implement using the KeyboardSymbolCode structure.
  SDL_Keycode sym;

  /// \brief An enumeration of key modifier masks; see also: the SDL2 wiki
  /// documentation page for [SDL_Keymod](https://wiki.libsdl.org/SDL_Keymod).
  uint16 mod;
};

/// \brief A structure containing information on a keyboard event.
///
/// \remarks  SDL_KEYDOWN; event type 768 (dec),
///           SDL_KEYUP; event type 769 (dec).
struct KeyboardEvent
{
  SDL_Scancode scan_code;

  /// \brief Symbol of the key press event.
  ///
  /// \todo Implement using the KeyboardSymbol structure.
  // SDL_Keysym sym;
  int32 sym;

  /// \brief An enumeration of key modifier masks; see also: the SDL2 wiki
  /// documentation page for [SDL_Keymod](https://wiki.libsdl.org/SDL_Keymod).
  uint16 mod;

  /// \brief The state of the key press event.
  ///
  /// \remarks SDL_PRESSED or SDL_RELEASED.
  uint8 state;

  /// \brief Non-zero if this is a repeating key press event.
  uint8 repeat;

  /// \brief The identifier of the window at the moment of the event.
  uint32 window_id;

  void dump( void ) const
  {
    NOM_DUMP( scan_code );
    NOM_DUMP( sym );
    NOM_DUMP( mod );
    NOM_DUMP( NOM_SCAST( int, state ) );
    NOM_DUMP( NOM_SCAST( int, repeat ) );
    NOM_DUMP( window_id );
  }
};

/// \brief A structure containing information on a mouse event.
///
/// \remarks SDL_MOUSEMOTION; event type 1024 (dec).
struct MouseMotionEvent
{
  /// \brief The mouse instance identifier, or SDL_TOUCH_MOUSEID.
  ///
  /// \remarks 'id' may be SDL_TOUCH_MOUSEID, for events that were generated by
  /// a touch input device, and not a real mouse. (You may want to ignore such
  /// events, if your application already handles SDL_TouchFingerEvent).
  uint32 id;

  /// \brief The X coordinate, relative to the nom::Window instance.
  int32 x;

  /// \brief The Y coordinate, relative to the nom::Window instance.
  int32 y;

  /// \brief The relative motion in the X coordinate direction.
  int32 x_rel;

  /// \brief The relative motion in the Y coordinate direction.
  int32 y_rel;

  /// \brief The state of the button.
  ///
  /// \remarks The state is a 32-bit button bit-mask of the current button
  /// state; the following masks are: SDL_BUTTON_LMASK, SDL_BUTTON_MMASK,
  /// SDL_BUTTON_RMASK, SDL_BUTTON_X1MASK, SDL_BUTTON_X2MASK.
  uint8 state;

  /// \brief The identifier of the window at the moment of the event.
  uint32 window_id;

  void dump( void ) const
  {
    NOM_DUMP( id );
    NOM_DUMP( x );
    NOM_DUMP( y );
    NOM_DUMP( x_rel );
    NOM_DUMP( y_rel );
    NOM_DUMP( NOM_SCAST( int, state ) );
    NOM_DUMP( window_id );
  }
};

/// \brief A structure containing information on a mouse button event.
///
/// \remarks  SDL_MOUSEBUTTONDOWN; event type 1025 (dec),
///           SDL_MOUSEBUTTONUP; event type 1026 (dec).
struct MouseButtonEvent
{
  /// \brief The mouse instance identifier, or SDL_TOUCH_MOUSEID.
  ///
  /// \remarks 'id' may be SDL_TOUCH_MOUSEID, for events that were generated by
  /// a touch input device, and not a real mouse. (You may want to ignore such
  /// events, if your application already handles SDL_TouchFingerEvent).
  ///
  /// \note ev->button.which
  uint32 id;

  /// \brief The X coordinate, relative to the nom::Window instance.
  int32 x;

  /// \brief The Y coordinate, relative to the nom::Window instance.
  int32 y;

  /// \brief The button that has changed.
  ///
  /// \remarks This field may be one of: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE,
  /// SDL_BUTTON_RIGHT, SDL_BUTTON_X1 or SDL_BUTTON_X2.
  uint8 button;

  /// \brief The state of the button.
  ///
  /// \remarks SDL_PRESSED or SDL_RELEASED.
  uint8 state;

  /// \brief Value containing how many mouse buttons were clicked.
  ///
  /// \remarks One (1) for single-click, two (2) for double-click, and so on.
  uint8 clicks;

  /// \brief The identifier of the window at the moment of the event.
  ///
  /// \note ev->button.windowID
  uint32 window_id;

  void dump( void ) const
  {
    NOM_DUMP( id );
    NOM_DUMP( x );
    NOM_DUMP( y );
    NOM_DUMP( NOM_SCAST( int, button ) );
    NOM_DUMP( NOM_SCAST( int, state ) );
    NOM_DUMP( NOM_SCAST( int, clicks ) );
    NOM_DUMP( window_id );
  }
};

/// \brief A structure containing information on a mouse wheel event.
///
/// \remarks SDL_MOUSEWHEEL; event type 1027 (dec).
struct MouseWheelEvent
{
  /// \brief The mouse instance identifier, or SDL_TOUCH_MOUSEID.
  ///
  /// \remarks 'id' may be SDL_TOUCH_MOUSEID, for events that were generated by
  /// a touch input device, and not a real mouse. (You may want to ignore such
  /// events, if your application already handles SDL_TouchFingerEvent).
  uint32 id;

  /// \brief X axis value; relative to the direction of the wheel.
  int32 x;

  /// \brief Y axis value, relative to the direction of the wheel.
  int32 y;

  // /// \brief The direction of the wheel.
  // ///
  // /// \remarks The axis of the wheel event; zero (0) for X-axis and one (1)
  // /// for Y-axis.
  // ///
  // /// \note This is only implemented for internal usage within
  // /// nom::InputMapper.
  // ///
  // /// \see nom::MouseWheelAction
  // uint8 axis;

  /// \brief The identifier of the window at the moment of event.
  uint32 window_id;

  void dump( void ) const
  {
    NOM_DUMP( id );
    NOM_DUMP( x );
    NOM_DUMP( y );
    NOM_DUMP( window_id );
  }
};

/// \brief A structure containing information on a joystick axis motion event.
///
/// \remarks SDL_JOYAXISMOTION; event type 1536 (dec).
struct JoystickAxisEvent
{
  /// \brief Index of the joystick that reported the event.
  SDL_JoystickID id;

  /// \brief Index of the axis on the device.
  uint8 axis;

  /// \brief The current position of the axis.
  ///
  /// \remarks The range of the axis value is in between -32,768 to 32,767.
  int16 value;

  /// \brief Identifier of the window at the moment of the event.
  // uint32 window_id;

  void dump( void ) const
  {
    NOM_DUMP( id );
    NOM_DUMP( NOM_SCAST( int, axis ) );
    NOM_DUMP( value );
  }
};

/// \brief A structure containing information on a joystick button event.
///
/// \remarks  SDL_JOYBUTTONDOWN; event type 1539 (dec),
///           SDL_JOYBUTTONUP; event type 1540 (dec).
struct JoystickButtonEvent
{
  /// \brief Index of the joystick that reported the event.
  SDL_JoystickID id;

  /// \brief The index of the button.
  uint8 button;

  /// \brief The state of the button.
  ///
  /// \remarks SDL_PRESSED or SDL_RELEASED.
  uint8 state;

  void dump( void ) const
  {
    NOM_DUMP( id );
    NOM_DUMP( NOM_SCAST( int, button ) );
    NOM_DUMP( NOM_SCAST( int, state ) );
  }
};

/// \remarks SDL_JOYDEVICEADDED; event type 1541 (dec).
struct JoystickConnectedEvent
{
  /// \brief Index of the joystick that reported the event.
  SDL_JoystickID id;

  void dump( void ) const
  {
    NOM_DUMP( id );
  }
};

/// \remarks SDL_JOYDEVICEREMOVED; event type 1542 (dec).
struct JoystickDisconnectedEvent
{
  /// \brief Index of the joystick that reported the event.
  SDL_JoystickID id;

  void dump( void ) const
  {
    NOM_DUMP( id );
  }
};

/// \brief A structure containing information on a finger (touch) event.
struct FingerEvent
{
  /// \brief The finger index identifier.
  SDL_FingerID id;

  /// \brief The X coordinate of the event; normalized 0..1.
  float x;

  /// \brief The Y coordinate of the event; normalized 0..1.
  float y;

  /// \brief The quantity of the pressure applied; normalized 0..1.
  float pressure;

  void dump( void ) const
  {
    NOM_DUMP( id );
    NOM_DUMP( x );
    NOM_DUMP( y );
    NOM_DUMP( pressure );
  }
};

/// \brief A structure containing information on a finger touch event.
///
/// \remarks SDL_FINGERMOTION, SDL_FINGERDOWN, or SDL_FINGERUP.
struct FingerTouchEvent
{
  /// \brief The touch device index identifier.
  SDL_TouchID id;

  /// \brief The finger index identifier.
  FingerEvent finger;

  /// \brief The X coordinate of the event; normalized 0..1.
  float x;

  /// \brief The Y coordinate of the event; normalized 0..1.
  float y;

  /// \brief The distance moved in the X axis; normalized 0..1.
  float dx;

  /// \brief The distance moved in the Y axis; normalized 0..1.
  float dy;

  /// \brief The quantity of the pressure applied; normalized 0..1.
  float pressure;

  void dump( void ) const
  {
    NOM_DUMP( id );
    NOM_DUMP( finger.id );
    finger.dump();
    NOM_DUMP( x );
    NOM_DUMP( y );
    NOM_DUMP( dx );
    NOM_DUMP( dy );
    NOM_DUMP( pressure );
  }
};

/// \brief A structure containing information on a multi-finger gesture (touch)
/// event.
///
/// \remarks SDL_MULTIGESTURE.
struct GestureEvent
{
  /// \brief The touch device index identifier.
  SDL_TouchID id;

  /// \brief The X coordinate of the event.
  float x;

  /// \brief The Y coordinate of the event.
  float y;

  /// \brief ...???
  float dTheta;

  /// \brief ...???
  float dDist;

  /// \brief The number of fingers used in the gesture event.
  uint16 num_fingers;

  void dump( void ) const
  {
    NOM_DUMP( id );
    NOM_DUMP( dTheta );
    NOM_DUMP( dDist );
    NOM_DUMP( x );
    NOM_DUMP( y );
    NOM_DUMP( num_fingers );
  }
};

/// \brief A structure containing information on a Drag 'N' Drop event.
///
/// \remarks SDL_DROPFILE; event type 4096 (dec).
struct DragDropEvent
{
  /// \brief The path of the file dropped onto the nom::Window.
  const char* file_path;

  void dump( void ) const
  {
    if( file_path != nullptr )
    {
      NOM_DUMP( file_path );
    }
  }
};

/// \brief A structure containing information on a text input event.
///
/// \remarks SDL_TEXTINPUT; event type 771 (dec).
struct TextInputEvent
{
  /// \brief The text input.
  char text[MAX_TEXT_INPUT_LENGTH];

  /// \brief The identifier of the window at the moment of the event.
  uint32 window_id;

  void dump( void ) const
  {
    NOM_DUMP( text );
    NOM_DUMP( window_id );
  }
};

/// \brief A structure containing information on a text editing event.
///
/// \remarks SDL_TEXTEDITING.
struct TextEditingEvent
{
  /// \brief The text being edited.
  char text[SDL_TEXTEDITINGEVENT_TEXT_SIZE];

  /// \brief The location to begin editing from.
  int32 start;

  /// \brief The number of characters to edit from the start point.
  int32 length;

  /// \brief The identifier of the window at the moment of the event.
  uint32 window_id;

  void dump( void ) const
  {
    NOM_DUMP( text );
    NOM_DUMP( start );
    NOM_DUMP( length );
    NOM_DUMP( window_id );
  }
};

/// \brief A structure containing information on an user event.
///
/// \remarks SDL_USEREVENT.
struct UserEvent
{
  /// \brief Convenience getter for nom::EventCallback objects stored in the
  /// data2 field.
  EventCallback* get_callback( void ) const
  {
    return NOM_SCAST( EventCallback*, data2 );
  }

  /// \brief User defined event code.
  int32 code;

  /// \brief User defined data pointer.
  void* data1;

  /// \brief User defined data pointer.
  void* data2;

  /// \brief The identifier of the window at the moment of the event.
  uint32 window_id;

  void dump( void ) const
  {
    NOM_DUMP( code );
    NOM_DUMP( window_id );
  }
};

/// \brief Event handling types.
struct Event
{
  union
  {
    WindowEvent window;
    KeyboardEvent key;
    MouseMotionEvent motion;
    MouseButtonEvent mouse;
    MouseWheelEvent wheel;
    JoystickButtonEvent jbutton;
    JoystickAxisEvent jaxis;
    JoystickConnectedEvent jconnected;
    JoystickDisconnectedEvent jdisconnected;
    FingerTouchEvent touch;
    GestureEvent gesture;
    DragDropEvent drop;
    TextInputEvent text;
    TextEditingEvent edit;
    UserEvent user;
  };

  /// \brief The event type.
  ///
  /// \todo Change to enumeration type?
  uint32 type;

  /// \brief The recorded time at the moment of the event.
  uint32 timestamp;

  void dump( void ) const
  {
    NOM_DUMP( type );
    NOM_DUMP( timestamp );
  }
};

} // namespace nom

#endif // include guard defined

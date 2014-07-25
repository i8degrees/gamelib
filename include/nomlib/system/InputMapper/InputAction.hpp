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
#ifndef NOMLIB_SYSTEM_INPUT_MAPPER_INPUT_ACTION_HPP
#define NOMLIB_SYSTEM_INPUT_MAPPER_INPUT_ACTION_HPP

#include "nomlib/config.hpp"
#include "nomlib/system/EventCallback.hpp"
#include "nomlib/system/Event.hpp"

namespace nom {

/// \brief Base class for mapping an action to an input device
///
/// \remarks See also nom::InputMapper, nom::InputContext, nom::InputState
class InputAction
{
  public:
    /// \brief Destructor.
    virtual ~InputAction( void );

    /// \brief Get the underlying event associated with the input action.
    ///
    /// \remarks The event returned is "fake" -- it is used as the action's data
    /// fields for matching against the real user input.
    virtual const Event& event( void ) const;

    /// \brief Get the callback assigned to the input action.
    const EventCallback& callback( void ) const;

    void set_callback( const EventCallback& delegate );

    /// \brief C++ functor; execute the callback assigned for the input action.
    void operator() ( const Event& evt ) const;

    /// \brief Diagnostic output of the object state.
    virtual void dump( void ) const;

  protected:
    /// \brief The event type and relevant input data; the criteria is used to
    /// match against user input.
    Event event_;

  private:
    /// \brief The delegate to call upon a successful action to event match.
    EventCallback callback_;
};

/// \brief A structure containing information on a keyboard action.
struct KeyboardAction: public InputAction
{
  /// \brief Destructor.
  virtual ~KeyboardAction( void );

  /// \brief Constructor for initializing an object to a valid action state.
  KeyboardAction( uint32 type, int32 sym );

  /// \brief Constructor for initializing an object to a valid action state.
  KeyboardAction( uint32 type, int32 sym, uint16 mod );

  /// \brief Constructor for initializing an object to a valid action state.
  KeyboardAction( uint32 type, int32 sym, uint16 mod, uint8 repeat );

  /// \brief Diagnostic output of the object state.
  void dump( void ) const;
};

/// \brief A structure containing information on a mouse button action.
struct MouseButtonAction: public InputAction
{
  /// \brief Destructor.
  virtual ~MouseButtonAction( void );

  /// \brief Constructor for initializing an object to a valid action state.
  MouseButtonAction( uint32 type, uint8 button );

  /// \brief Diagnostic output of the object state.
  void dump( void ) const;
};

/// \brief A structure containing information on a mouse wheel action.
struct MouseWheelAction: public InputAction
{
  /// \brief Left-right and up-down axis.
  ///
  /// \remarks Conceptually, a wheel action event has been modeled similarly to a
  /// joystick axis -- note the axis field -- with the left-right axis being zero
  /// (0) and the up-down axis being one (1).
  enum: int
  {
    INVALID = -1,
    AXIS_X = 0,
    AXIS_Y = 1
  };

  /// \brief Sensitivity ranges to action mapping.
  enum: int
  {
    UP = 1,
    DOWN = -1,
    RIGHT = -1,
    LEFT = 1
  };

  /// \brief An enumeration of the invalid state(s).
  enum: int
  {
    null = 0
  };

  /// \brief Destructor.
  virtual ~MouseWheelAction( void );

  /// \brief Constructor for initializing an object to a valid action state.
  MouseWheelAction( uint32 type, uint8 axis, int32 value );

  /// \brief Diagnostic output of the object state.
  void dump( void ) const;
};

/// \brief A structure containing information on a joystick button action.
///
/// \todo Implement button state (SDL_PRESSED or SDL_RELEASED).
struct JoystickButtonAction: public InputAction
{
  /// \brief Destructor.
  virtual ~JoystickButtonAction( void );

  /// \brief Constructor for initializing an object to a valid action state.
  JoystickButtonAction( SDL_JoystickID id, uint32 type, uint8 button );

  /// \brief Diagnostic output of the object state.
  void dump( void ) const;
};

/// \brief A structure containing information on a joystick axis motion action.
struct JoystickAxisAction: public InputAction
{
  /// \brief Destructor.
  virtual ~JoystickAxisAction( void );

  /// \brief Constructor for initializing an object to a valid action state.
  JoystickAxisAction( SDL_JoystickID id, uint32 type, uint8 axis, int16 value );

  /// \brief Diagnostic output of the object state.
  void dump( void ) const;
};

} // namespace nom

#endif // include guard defined

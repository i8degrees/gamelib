/******************************************************************************
    GameStates.hpp

    Game states interface class

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_GAMESTATES_HEADERS
#define NOMLIB_GAMESTATES_HEADERS

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "GameState.h" // abstract class for our interface

#include "gamelib.h"

namespace nom
{
  class GameStates
  {
    public:
      /// Pause method is implemented in each GameState derived class
      /// Resume method is implemented in each GameState derived class

      static void HandleInput ( void );
      static void Update ( void );
      static void Draw ( void* video_buffer );

      static void ChangeState ( std::unique_ptr<GameState> state );
      static void PushState ( std::unique_ptr<GameState> state );
      static void PopState ( void );
      static void PopStateThenChangeState ( std::unique_ptr<GameState> state );
    private:
      GameStates ( void );
  };
}

#endif // NOMLIB_GAMESTATES_HEADERS defined

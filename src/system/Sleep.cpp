/******************************************************************************

    Sleep / delay interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/system/Sleep.hpp"

namespace nom {

void sleep ( uint32 milliseconds )
{
  SDL_Delay ( std::max ( milliseconds, static_cast<uint32> ( 10 ) ) );
}


} // namespace nom

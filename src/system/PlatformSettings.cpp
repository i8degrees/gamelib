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
#include "nomlib/system/PlatformSettings.hpp"

// Generic (platform-agnostic) implementation of PlatformSettings

namespace nom {

// Static initialization
bool PlatformSettings::initialized_ = false;

void PlatformSettings::initialize( void )
{
  if( PlatformSettings::initialized() == false )
  {
    NOM_LOG_INFO( NOM, "PlatformSettings interface was not yet initialized. Initializing..." );

    nom::init_third_party( InitHints::SDL2_IMAGE | InitHints::SDL2 | InitHints::SDL2_TTF );

    PlatformSettings::enumerate_fonts();
    PlatformSettings::initialized_ = true;
  }
}

Font* PlatformSettings::find_system_font( const std::string& key )
{
  // Ensure that we are initialized before using
  PlatformSettings::initialize();

  return SystemFonts::cache().load_resource( key );
}

// Private scope

bool PlatformSettings::initialized( void )
{
  return PlatformSettings::initialized_;
}

} // namespace nom

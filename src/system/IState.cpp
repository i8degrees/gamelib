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
#include "nomlib/system/IState.hpp"

// Forward declarations
#include "nomlib/graphics/RenderWindow.hpp"
#include "nomlib/system/Event.hpp"

namespace nom {

IState::IState( void ) :
  id_{ 0 },
  timestamp_{ nom::ticks() },
  flags_{ 0 },
  type_{ IState::Type::Parent }
{
  //NOM_LOG_TRACE ( NOM );
}

IState::~IState( void )
{
  //NOM_LOG_TRACE ( NOM );
}

IState::IState( uint32 id ) :
  id_{ id },
  timestamp_{ nom::ticks() },
  flags_{ 0 },
  type_{ IState::Type::Parent }
{
  //NOM_LOG_TRACE ( NOM );
}

IState::IState( uint32 id, uint32 flags, IState::Type type ) :
  id_{ id },
  timestamp_{ nom::ticks() },
  flags_{ flags },
  type_{ type }
{
  //NOM_LOG_TRACE ( NOM );
}

uint32 IState::id ( void ) const
{
  return this->id_;
}

uint32 IState::timestamp ( void ) const
{
  return this->timestamp_;
}

uint32 IState::flags ( void ) const
{
  return this->flags_;
}

IState::Type IState::type( void ) const
{
  return this->type_;
}

bool IState::on_event(const Event& ev)
{
  // User-defined virtual

  return false;
}

void IState::on_update ( float )
{
  // User-defined virtual
}

void IState::on_draw( RenderWindow& target )
{
  // User-defined virtual
}

void IState::on_init ( void_ptr )
{
  // User-defined virtual
}

void IState::on_exit ( void_ptr )
{
  // User-defined virtual
}

void IState::on_pause ( void_ptr )
{
  // User-defined virtual
}

void IState::on_resume ( void_ptr )
{
  // User-defined virtual
}

} // namespace nom

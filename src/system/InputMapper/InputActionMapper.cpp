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
#include "nomlib/system/InputMapper/InputActionMapper.hpp"

namespace nom {

InputActionMapper::InputActionMapper( void )
{
  // NOM_LOG_TRACE( NOM );
}

InputActionMapper::~InputActionMapper( void )
{
  // NOM_LOG_TRACE( NOM );
}

const InputActionMapper::ActionMap& InputActionMapper::get( void ) const
{
  return this->input_map_;
}

bool InputActionMapper::insert( const std::string& key, const InputAction& action )
{
  ActionMap::const_iterator it = this->input_map_.insert( InputAction::Pair( key, action ) );

  // No dice; the insertion failed for some reason!
  if( it == this->input_map_.end() )
  {
    NOM_LOG_ERR( NOM, "Could not insert action with key: " + key );
    return false;
  }

  return true;
}

bool InputActionMapper::erase( const std::string& key )
{
  ActionMap::const_iterator itr = this->input_map_.find( key );

  // No match found
  if( itr == this->input_map_.end() )
  {
    return false;
  }
  else // Match found; remove this input mapping
  {
    this->input_map_.erase( itr );

    return true;
  }

  return false;
}

void InputActionMapper::dump( void ) const
{
  for( ActionMap::const_iterator itr = this->input_map_.begin(); itr != this->input_map_.end(); ++itr )
  {
    if( itr->second.key != nullptr )
    {
      itr->second.key->dump();
    }
    else if ( itr->second.mouse != nullptr )
    {
      itr->second.mouse->dump();
    }
    else if ( itr->second.jbutton != nullptr )
    {
      itr->second.jbutton->dump();
    }
    else
    {
      NOM_LOG_ERR( NOM, "Invalid input mapping state." );
    }
  }
}

} // namespace nom

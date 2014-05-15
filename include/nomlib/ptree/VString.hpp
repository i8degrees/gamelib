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
#ifndef NOMLIB_SYSTEM_PTREE_VSTRING_HPP
#define NOMLIB_SYSTEM_PTREE_VSTRING_HPP

#include <cstring>
#include <string>

#include "nomlib/config.hpp"
#include "nomlib/system/helpers.hpp" // priv::duplicate_string
#include "nomlib/ptree/ptree_config.hpp"
#include "nomlib/ptree/ptree_forwards.hpp"

namespace nom {

typedef uint ArrayIndex;

class VString
{
  public:
    typedef VString SelfType;

    /// \brief Default constructor.
    VString( void );

    /// \brief Destructor.
    ~VString( void );

    /// \brief Constructor for array element index.
    VString( ArrayIndex index );

    /// \brief Constructor for key member pair.
    VString( const char* key );

    /// \brief Constructor for key member pair.
    VString( const std::string& key );

    /// \brief Copy constructor.
    VString( const SelfType& copy );

    /// \brief Copy assignment operator.
    SelfType& operator =( const SelfType& other );

    void swap( VString& other );

    /// \brief Short-hand for checking if class variable value_ is nullptr.
    bool valid( void ) const;

    /// \note Required implementation for usage inside a std::map template.
    bool operator <( const VString& other ) const;

    /// \note Required implementation for usage inside a std::map template.
    bool operator ==( const VString& other ) const;

    /// \brief Getter for the C string (char*) copy of the stored member key.
    const char* c_str( void ) const;

    // / \brief Getter for std::string copy of the stored member key.
    const std::string get_string( void ) const;

    /// \brief Getter for the contained array element index.
    ///
    /// \returns Index of the element.
    ArrayIndex index( void ) const;

  private:
    const char* value_;
    ArrayIndex index_;
};

} // namespace nom

#endif // include guard defined

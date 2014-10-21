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
#ifndef NOMLIB_SYSTEM_PTREE_VALUE_ITERATOR_BASE_HPP
#define NOMLIB_SYSTEM_PTREE_VALUE_ITERATOR_BASE_HPP

#include <vector>
#include <iterator>

#include "nomlib/config.hpp"
#include "nomlib/ptree/ptree_config.hpp"
#include "nomlib/ptree/ptree_types.hpp"
#include "nomlib/ptree/ptree_forwards.hpp"
#include "nomlib/ptree/Value.hpp"

namespace nom {

class ValueIteratorBase
{
  public:
    typedef ValueIteratorBase SelfType;
    typedef ValueIteratorBase* RawPtr;
    typedef nom::size_type DifferenceType;
    typedef Value& ValueTypeReference;
    typedef Value* ValueTypePointer;

    enum IteratorType
    {
      Null = 0,         // Type 0 (default)
      ObjectValues      // Type 7
    };

    /// \brief Default constructor.
    ValueIteratorBase( void );

    /// \brief Destructor.
    virtual ~ValueIteratorBase( void );

    ValueIteratorBase( const ObjectIterator& itr );

    /// \brief query validity of the object.
    ///
    /// \returns The object is always assumed to be invalid when
    /// Value::ValueType::NullValue is the set type (set during construction).
    bool valid( void ) const;

    /// \brief Obtain the index of the referenced value.
    ///
    /// \returns The array element's index on success, or -1 if the referenced
    /// value is not an array element.
    /*ArrayIndex*/int index( void ) const;

    /// \brief Obtain the current index or member key value.
    ///
    /// \returns Member key of the referenced value on success, nullptr if not
    /// not an object type or the member key is not found.
    ///
    /// \todo Try handling the return of the key (string) or index (integer)
    /// more elegantly? (Presently, we are converting index values to a string).
    const char* key( void ) const;

    /// \brief Query if the member key exists at the current position of the
    /// iterator object.
    bool key( const std::string& member ) const;

    Value key_v2( void ) const;

    bool operator ==( const SelfType& other ) const;

    bool operator !=( const SelfType& other ) const;

    DifferenceType operator -( const SelfType& other ) const;

  protected:
    void copy( const SelfType& other );
    ValueTypeReference dereference( void ) const;
    ValueTypePointer pointer( void ) const;
    void increment( void );
    void decrement( void );
    DifferenceType distance( const SelfType& other ) const;

  private:
    enum IteratorType type( void ) const;

    /// \brief A pointer to the object type iterator
    ObjectIterator object_;

    enum IteratorType type_;
};

} // namespace nom

#endif // include guard defined

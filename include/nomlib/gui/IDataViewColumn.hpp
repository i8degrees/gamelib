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
#ifndef NOMLIB_GUI_IDATAVIEW_COLUMN_HPP
#define NOMLIB_GUI_IDATAVIEW_COLUMN_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/graphics/fonts/IFont.hpp"

namespace nom {

/// \brief Abstract base class for DataViewColumn types
class IDataViewColumn
{
  public:
    typedef IDataViewColumn SelfType;

    typedef SelfType* RawPtr;
    typedef std::unique_ptr<SelfType> UniquePtr;
    typedef std::shared_ptr<SelfType> SharedPtr;

    /// Column text alignment options.
    ///
    /// \TODO Relocate me to a common area!
    enum Alignment
    {
      Left = 0,  // Default
      Center,
      Right
    };

    IDataViewColumn( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

    virtual ~IDataViewColumn( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

    virtual uint id( void ) const = 0;

    virtual const std::string& title( void ) const = 0;

    virtual int width( void ) const = 0;

    virtual IDataViewColumn::Alignment alignment( void ) const = 0;

    virtual bool sortable( void ) const = 0;

    virtual bool hidden( void ) const = 0;

    virtual const IFont::shared_ptr& font( void ) const = 0;

    // TODO: setters
};

} // namespace nom

#endif // include guard defined

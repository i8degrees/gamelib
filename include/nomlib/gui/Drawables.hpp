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
#ifndef NOMLIB_GUI_DRAWABLES_HPP
#define NOMLIB_GUI_DRAWABLES_HPP

#include <vector>
#include <memory>
#include <iterator>

#include "nomlib/config.hpp"
#include "nomlib/graphics/IDrawable.hpp"

namespace nom {

class Drawables
{
  public:
    typedef Drawables self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    typedef IDrawable value_type;
    typedef std::vector<value_type::shared_ptr> container;

    typedef container::iterator iterator;
    typedef container::const_iterator const_iterator;

    /// \brief Default constructor.
    Drawables( void );

    /// \brief Destructor.
    ~Drawables( void );

    /// \brief Constructor to ...
    Drawables( const Drawables::container& object );

    const container& drawables( void ) const;

    /// \brief Getter for internal updated status.
    bool updated( void ) const;

    /// \brief Set the internal updated status.
    void set_updated( bool state );

    void assign( const container& objects );

    void insert( uint pos, const value_type::raw_ptr object );

    void push_back( const value_type::raw_ptr object );

    const value_type::shared_ptr& at( uint pos );

    Drawables::iterator erase( uint pos );

    // bool erase( const value_type::raw_ptr object );

    bool empty( void ) const;
    uint size( void ) const;
    void clear( void );

    /// \brief Iterator access to the beginning of the render list.
    ///
    /// \returns Non-modifiable (const) nom::Drawables object iterator.
    Drawables::const_iterator begin( void ) const;

    /// \brief Iterator access to the end of the render list.
    ///
    /// \returns Non-modifiable (const) nom::Drawables object iterator.
    Drawables::const_iterator end( void ) const;

    /// \brief Iterator access to the beginning of the render list.
    ///
    /// \returns nom::Drawables object iterator.
    Drawables::iterator begin( void );

    /// \brief Iterator access to the end of the render list.
    ///
    /// \returns nom::Drawables object iterator.
    Drawables::iterator end( void );

  private:
    container drawables_;

    /// \brief Internal object state; we re-create drawable objects every time
    /// the internal state changes in order to save as many CPU cycles.
    bool updated_;
};

} // namespace nom

#endif // include guard defined

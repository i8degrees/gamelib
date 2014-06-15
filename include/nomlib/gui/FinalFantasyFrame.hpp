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
#ifndef NOMLIB_GUI_FINAL_FANTASY_FRAME_HPP
#define NOMLIB_GUI_FINAL_FANTASY_FRAME_HPP

#include "nomlib/config.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/gui/Drawables.hpp"
#include "nomlib/graphics/shapes/Line.hpp"
#include "nomlib/graphics/IDrawable.hpp"

namespace nom {

/// \brief Final Fantasy theme styled border
class FinalFantasyFrame: public Transformable
{
  public:
    typedef FinalFantasyFrame self_type;

    typedef self_type* raw_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;

    /// \brief Default constructor; initialize the object to an invalid object
    /// state -- position and size objects are invalid (null).
    FinalFantasyFrame( void );

    /// \brief Destructor.
    ~FinalFantasyFrame( void );

    FinalFantasyFrame (
                        const Point2i& pos,
                        const Size2i& size
                      );

    /// \brief Implements required IDrawable::clone method.
    IDrawable::raw_ptr clone( void ) const;

    /// \brief Query the validity of the object for rendering.
    ///
    /// \remarks A valid object must have both the positioning & size bounding
    /// coordinates be set to a non-null object value.
    bool valid( void ) const;

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type( void ) const;

    /// \brief Get the unaffected rendering area coordinates of the border.
    const IntRect& frame_bounds( void ) const;

    /// \brief Implements IDrawable::draw
    void draw( RenderTarget& target ) const;

    /// \brief Set the intended boundaries of the "client area", meaning the
    /// unaffected rendering area of the rendered border.
    ///
    /// \remarks The boundary coordinates should exclude the rendering area used
    ///  by the rendering of the border, leaving only what is free (think: safe)
    /// for others to render on.
    ///
    /// \note This information is provided as a convenience for visual
    /// interfaces that require such detial about their environment, such as
    /// a GUI engine -- nom::UIWidget and friends.
    void set_frame_bounds( const IntRect& bounds );

  private:
    /// \brief Implements IDrawable::update
    void update( void );

    /// \brief The rendering area unaffected by the rendered border.
    IntRect frame_bounds_;

    /// \remarks This object internally tracks its object state for when to
    /// clear its cached data and update.
    Drawables drawables_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::FinalFantasyFrame
/// \ingroup gui
///
/// See also: nom::FinalFantasyDecorator.
///

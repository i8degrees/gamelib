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
#include "nomlib/librocket/DecoratorFinalFantasyFrame.hpp"

#include <Rocket/Core/Math.h>
#include <Rocket/Core/Element.h>

// Private headers
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/gui/FinalFantasyDecorator.hpp"
#include "nomlib/librocket/RocketSDL2RenderInterface.hpp"

// Forward declarations
#include "nomlib/gui/IDecorator.hpp"

namespace nom {

DecoratorFinalFantasyFrame::DecoratorFinalFantasyFrame()
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);

  this->decorator_.reset( new FinalFantasyDecorator() );
}

DecoratorFinalFantasyFrame::~DecoratorFinalFantasyFrame()
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);
}

bool DecoratorFinalFantasyFrame::Initialise()
{
  return true;
}

Rocket::Core::DecoratorDataHandle DecoratorFinalFantasyFrame::GenerateElementData(Rocket::Core::Element* ROCKET_UNUSED_PARAMETER(element))
{
  ROCKET_UNUSED(element);

  return Rocket::Core::Decorator::INVALID_DECORATORDATAHANDLE;
}

void DecoratorFinalFantasyFrame::ReleaseElementData(Rocket::Core::DecoratorDataHandle ROCKET_UNUSED_PARAMETER(element_data))
{
  ROCKET_UNUSED(element_data);
}

void DecoratorFinalFantasyFrame::RenderElement(Rocket::Core::Element* element, Rocket::Core::DecoratorDataHandle ROCKET_UNUSED_PARAMETER(element_data))
{
  NOM_ASSERT( this->decorator_ != nullptr );
  if( this->decorator_ == nullptr ) return;

  ROCKET_UNUSED(element_data);
  Rocket::Core::Vector2f position = element->GetAbsoluteOffset(Rocket::Core::Box::PADDING);
  Rocket::Core::Vector2f size = element->GetBox().GetSize(Rocket::Core::Box::PADDING);

  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_GUI, "position:", position.x, position.y);
  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_GUI, "size:", size.x, size.y);

  nom::RocketSDL2RenderInterface* target = NOM_DYN_PTR_CAST( nom::RocketSDL2RenderInterface*, Rocket::Core::GetRenderInterface() );

  NOM_ASSERT( target != nullptr );
  if( target == nullptr ) return;

  const RenderWindow* context = target->window_;

  NOM_ASSERT( context != nullptr );
  if( context == nullptr ) return;

  // Not ready to render yet; this can happen when there is no width or height
  // defined in 'body.window'.
  if( size.x <= 0 || size.y <= 0 ) return;

  // Check for whether or not we need to update our decorator
  if( this->coords_.x != position.x || this->coords_.y != position.y || this->coords_.w != size.x || this->coords_.h != size.y )
  {
    // Update our coordinates to match new element coordinates
    this->coords_.x = position.x;
    this->coords_.y = position.y;
    this->coords_.w = size.x;
    this->coords_.h = size.y;

    // Keeps our decorator within positive bounds on the left side of the
    // window, else it will vanish on us
    if( this->coords_.x <= 0 )
    {
      int x_offset = abs( this->coords_.x );

      // We need to recalculate widths when the object is partially off-screen,
      // otherwise stretching of the layout occurs -- a possible bug in
      // libRocket?
      if( this->coords_.w > x_offset )  // Bad things happen if w < 0
      {
        this->coords_.w = this->coords_.w - x_offset;
        this->coords_.x = 0;
      }
      // else The object is entirely offscreen (this is valid)
    }

    decorator_->set_bounds( this->coords_ );
    decorator_->update();
  }

  decorator_->draw( *context );
}

} // namespace nom

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
#include "nomlib/graphics/MoveToAction.hpp"

// Private headers
#include "nomlib/math/math_helpers.hpp"

// Forward declarations
#include "nomlib/graphics/Texture.hpp"

namespace nom {

MoveToAction::MoveToAction( const std::shared_ptr<Texture>& action,
                            const Point2i& delta, real32 duration ) :
  total_displacement_(delta)
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ANIMATION,
                      NOM_LOG_PRIORITY_VERBOSE );

  this->set_duration(duration);
  this->curr_frame_ = 0.0f;
  this->drawable_ = action;
}

MoveToAction::~MoveToAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ANIMATION,
                      NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<MoveToAction::derived_type> MoveToAction::clone() const
{
  return( std::unique_ptr<self_type>( new self_type(*this) ) );
}

IActionObject::FrameState
MoveToAction::update(real32 t, const Point2i& b, const Point2i& c, real32 d)
{
  // The current frame to displace
  real32 delta_time = t;

  // Total duration of the animation displacement
  const real32 duration = d;

  // Initial starting value; this is the first frame to displace from
  const Point2i initial_position(b);

  // Total displacement over time
  const Point2i total_displacement(c);

  // The computed displacement of the frame
  Point2f displacement(Point2f::zero);

  // Clamp delta values that go beyond the time duration bounds; this adds
  // stability to variable time steps
  if( delta_time > (duration / this->speed() ) ) {
    delta_time = duration / this->speed();
  }

  // initial starting values to displace from
  const real32 b1 = initial_position.x;
  const real32 b2 = initial_position.y;

  // total change in value (applied over time)
  real32 c1 = 0;
  real32 c2 = 0;
  c1 = total_displacement.x;
  c2 = total_displacement.y;

  // Account for the translating of negative X and Y displacements
  // if( total_displacement.x >= b1 ) {
  //   c1 = total_displacement.x - b1;
  // } else {
  //   c1 = total_displacement.x;
  // }

  // if( total_displacement.y >= b2 ) {
  //   c2 = total_displacement.y - b2;
  // } else {
  //   c2 = total_displacement.y;
  // }

  NOM_ASSERT(this->timing_mode() != nullptr);

  // Apply speed scalar onto current frame time
  real32 frame_time = delta_time * this->speed();

  displacement.x =
    this->timing_mode().operator()(frame_time, b1, c1, duration);
  displacement.y =
    this->timing_mode().operator()(frame_time, b2, c2, duration);

  // Update our internal elapsed frames counter (diagnostics)
  ++this->curr_frame_;

  auto drawable = this->drawable_.lock();
  if( drawable != nullptr ) {

    // State diagnostics
    Point2i drawable_pos = drawable->position();

    // Convert floating-point value to integer; it is critical that we round
    // the values so that values like 254.999984741 are represented as 255.00
    Point2i displacement_as_integer(Point2i::zero);

    if( total_displacement_.x != 0 ) {
      displacement_as_integer.x = nom::round_float<int>(displacement.x);
    } else {
      displacement_as_integer.x = drawable_pos.x;
    }

    if( total_displacement_.y != 0 ) {
      displacement_as_integer.y = nom::round_float<int>(displacement.y);
    } else {
      displacement_as_integer.y = drawable_pos.y;
    }

    drawable->set_position(displacement_as_integer);

    // Diagnostics
    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION, "[MoveToAction]",
                    "delta_time:", delta_time,
                    "frame_time:", frame_time,
                    "[elapsed frames]:", this->curr_frame_ );
    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION, "[MoveToAction]",
                    "position (input):", drawable_pos,
                    "displacement (output):", displacement_as_integer );

    // Extended diagnostics
    NOM_LOG_VERBOSE(  NOM_LOG_CATEGORY_ANIMATION, "[MoveToAction]",
                      "[b1]:", b1, "[c1]:", c1,
                      "[b2]:", b2, "[c2]:", c2 );
  }

  // Continue playing the animation only when we are inside our frame duration
  // bounds; this adds stability to variable time steps
  if( delta_time < (duration / this->speed() ) ) {
    this->status_ = FrameState::PLAY_NEXT_FRAME;
    return this->status_;
  } else {
    // Diagnostics
    this->last_frame(delta_time);

    this->status_ = FrameState::DONE;
    return this->status_;
  }
}

IActionObject::FrameState MoveToAction::next_frame(real32 delta_time)
{
  // Initialize timer and initial position
  this->first_frame(delta_time);

  delta_time = this->timer_.ticks();

  return this->update(  delta_time, this->initial_position_,
                        this->total_displacement_, this->duration() );
}

// NOTE: UNTESTED!!
IActionObject::FrameState MoveToAction::prev_frame(real32 delta_time)
{
  // Initialize timer and initial position
  this->first_frame(delta_time);

  delta_time = this->timer_.ticks();

  // Inverse of ::next_frame
  return this->update(  delta_time, this->initial_position_,
                        -(this->total_displacement_), this->duration() );
}

void MoveToAction::pause(real32 delta_time)
{
  this->timer_.pause();
}

void MoveToAction::resume(real32 delta_time)
{
  this->timer_.unpause();
}

void MoveToAction::rewind(real32 delta_time)
{
  // Reset elapsed frame (diagnostics)
  this->curr_frame_ = 0.0f;

  // Reset frame timing
  this->timer_.stop();

  // Reset starting displacement position
  auto drawable = this->drawable_.lock();
  if( drawable != nullptr && this->initial_position_ != Point2i::null ) {
    drawable->set_position(this->initial_position_);
  }
}

void MoveToAction::release()
{
  auto drawable = this->drawable_.lock();
  if( drawable != nullptr ) {
    drawable.reset();
  }
}

// Private scope

void MoveToAction::first_frame(real32 delta_time)
{
  if( this->timer_.started() == false ) {
    // Start frame timing
    this->timer_.start();
    delta_time = this->timer_.ticks();

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION,
                    "MoveToAction::BEGIN at", delta_time );
  }

  // NOTE: The initial position needs to be set on every frame of the action!

  // Initialize the initial displacement position; this is also necessary for
  // reversing the animation, repeating it, etc.
  auto drawable = this->drawable_.lock();
  if( drawable != nullptr ) {
    this->initial_position_ = drawable->position();
  }

  NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION,
                  "[MoveToAction]", "initial_position:",
                  this->initial_position_ );
}

void MoveToAction::last_frame(real32 delta_time)
{
  this->timer_.stop();
}

} // namespace nom

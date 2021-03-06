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
#include "nomlib/actions/SpriteBatchAction.hpp"

// Private headers
#include "nomlib/core/helpers.hpp"
#include "nomlib/math/math_helpers.hpp"

// Forward declarations
#include "nomlib/graphics/sprite/SpriteBatch.hpp"

namespace nom {

// Static initializations
const char* SpriteBatchAction::DEBUG_CLASS_NAME = "[SpriteBatchAction]:";

SpriteBatchAction::
SpriteBatchAction(  const std::shared_ptr<SpriteBatch>& drawable,
                    real32 frame_interval_seconds )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->elapsed_frames_ = 0.0f;
  this->frame_interval_ = frame_interval_seconds;
  this->total_displacement_ = 0.0f;
  this->initial_frame_ = 0;
  this->last_delta_ = 0.0f;

  this->drawable_ = drawable;
  if( this->drawable_ != nullptr ) {
    NOM_ASSERT(this->drawable_->frames() > 0);
    this->total_displacement_ = this->drawable_->frames();
  }

  real32 action_duration_seconds =
    (this->frame_interval_ * this->total_displacement_);
  this->set_duration(action_duration_seconds);
}

SpriteBatchAction::~SpriteBatchAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<IActionObject> SpriteBatchAction::clone() const
{
  return( nom::make_unique<self_type>( self_type(*this) ) );
}

IActionObject::FrameState
SpriteBatchAction::update(real32 t, real32 b, real32 c, real32 d)
{
  real32 frame_interval =
    this->frame_interval_ / this->speed();

  // The current frame to scale
  real32 delta_time = t;

  // Total duration of the action
  const real32 duration = d;

  // initial starting value
  const int b1 = b;

  // Total number of frames over time
  real32 c1 = c;

  // The computed texture frame to show next
  real32 displacement(0.0f);
  int displacement_as_integer = 0;

  // Clamp delta values that go beyond maximal duration
  if( delta_time > (duration / this->speed() ) ) {
    delta_time = duration / this->speed();
  }

  // Apply speed scalar onto current frame time
  real32 frame_time = delta_time * this->speed();

  NOM_ASSERT(this->timing_curve() != nullptr);

  displacement =
    this->timing_curve().operator()(frame_time, b1, c1, duration);
  NOM_ASSERT(displacement <= this->total_displacement_);
  NOM_ASSERT(displacement >= this->initial_frame_);

  if( delta_time >= (this->last_delta_ + frame_interval) &&
      delta_time < (duration / this->speed() ) )
  {
    this->elapsed_frames_ = displacement;
    this->last_delta_ = delta_time;

    displacement_as_integer =
      nom::round_float_down<int>(displacement);

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    "delta_time:", delta_time, "frame_time:", frame_time,
                    "[elapsed frames]:", this->elapsed_frames_,
                    "displacement (output):", displacement_as_integer );

    if( this->drawable_ != nullptr ) {
      this->drawable_->set_frame(displacement_as_integer);
    }
  }

  if( delta_time < (duration / this->speed() ) ) {
    this->set_status(FrameState::PLAYING);
  } else {
    this->last_frame(delta_time);
    this->set_status(FrameState::COMPLETED);
  }

  return this->status();
}

IActionObject::FrameState SpriteBatchAction::next_frame(real32 delta_time)
{
  delta_time = ( Timer::to_seconds( this->timer_.ticks() ) );

  this->first_frame(delta_time);

  return( this->update( delta_time, this->initial_frame_,
          this->total_displacement_, this->duration() ) );
}

IActionObject::FrameState SpriteBatchAction::prev_frame(real32 delta_time)
{
  delta_time = ( Timer::to_seconds( this->timer_.ticks() ) );

  this->first_frame(delta_time);

  return( this->update( delta_time, this->total_displacement_,
          -(this->total_displacement_), this->duration() ) );
}

void SpriteBatchAction::pause(real32 delta_time)
{
  this->timer_.pause();
}

void SpriteBatchAction::resume(real32 delta_time)
{
  this->timer_.unpause();
}

void SpriteBatchAction::rewind(real32 delta_time)
{
  this->elapsed_frames_ = 0.0f;
  this->last_delta_ = 0.0f;
  this->timer_.stop();
  this->set_status(FrameState::PLAYING);

  if( this->drawable_ != nullptr ) {
    this->drawable_->set_frame(this->initial_frame_);
  }
}

void SpriteBatchAction::release()
{
  if( this->drawable_ != nullptr ) {
    this->drawable_->release_texture();
  }

  this->drawable_.reset();
}

void SpriteBatchAction::first_frame(real32 delta_time)
{
  if( this->timer_.started() == false ) {
    this->last_delta_ = 0.0f;
    this->timer_.start();

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                    "BEGIN at", delta_time );

    if( this->drawable_ != nullptr ) {
      this->initial_frame_ = this->drawable_->frame();
    }

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION,
                    "[initial_frame]:", this->initial_frame_,
                    "[num_frames]:", this->total_displacement_,
                    "[frame_interval]:", this->frame_interval_ );
  }
}

void SpriteBatchAction::last_frame(real32 delta_time)
{
  this->timer_.stop();
}

} // namespace nom

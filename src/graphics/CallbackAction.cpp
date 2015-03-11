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
#include "nomlib/graphics/CallbackAction.hpp"

namespace nom {

CallbackAction::CallbackAction(const callback_type& func) :
  delegate_(func)
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ANIMATION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_duration(0.0f);
  this->curr_frame_ = 0.0f;
}

CallbackAction::CallbackAction(real32 duration, const callback_type& func) :
  delegate_(func)
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ANIMATION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_duration(duration);
  this->curr_frame_ = 0.0f;
}

CallbackAction::~CallbackAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ANIMATION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<CallbackAction::derived_type> CallbackAction::clone() const
{
  return( std::unique_ptr<self_type>( new self_type(*this) ) );
}

IActionObject::FrameState CallbackAction::next_frame(real32 delta_time)
{
  if( this->timer_.started() == false ) {
    // Start frame timing
    this->timer_.start();
    delta_time = this->timer_.ticks();

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION,
                    "CallbackAction::BEGIN at", delta_time );
  }

  delta_time = this->timer_.ticks();

  // Clamp delta values that go beyond the time duration bounds; this adds
  // stability to variable time steps
  if( delta_time > (this->duration() / this->speed() ) ) {
    delta_time = this->duration() / this->speed();
  }

  // Apply speed scalar onto current frame time
  real32 frame_time = delta_time * this->speed();

  NOM_ASSERT(this->delegate_ != nullptr);
  if( this->delegate_ != nullptr ) {
    this->delegate_.operator()();
  }

  // Internal diagnostics
  ++this->curr_frame_;

  // Continue waiting if we are inside our frame duration bounds; this adds
  // stability to variable time steps
  if( delta_time < (this->duration() / this->speed() ) ) {

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ANIMATION, "[CallbackAction]",
                    "delta_time:", delta_time, "frame_time:", frame_time,
                    "[elapsed frames]:", this->curr_frame_ );

    this->status_ = FrameState::PLAY_NEXT_FRAME;
    return this->status_;
  } else {
    this->status_ = FrameState::DONE;
    return this->status_;
  }
}

IActionObject::FrameState CallbackAction::prev_frame(real32 delta_time)
{
  return this->next_frame(delta_time);
}

void CallbackAction::pause(real32 delta_time)
{
  // Not supported
}

void CallbackAction::resume(real32 delta_time)
{
  // Not supported
}

void CallbackAction::rewind(real32 delta_time)
{
  // Not supported
}

void CallbackAction::release()
{
  // Nothing to free
}

} // namespace nom

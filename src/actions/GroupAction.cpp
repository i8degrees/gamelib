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
#include "nomlib/actions/GroupAction.hpp"

namespace nom {

enum FrameStateDirection
{
  NEXT_FRAME,
  PREV_FRAME
};

// Static initializations
const char* GroupAction::DEBUG_CLASS_NAME = "[GroupAction]:";

GroupAction::GroupAction( const action_list& actions,
                          const std::string& name )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_name(name);

  group_action entry;
  for( auto itr = actions.begin(); itr != actions.end(); ++itr ) {

    entry.action = *itr;
    entry.status = FrameState::PLAYING;
    this->actions_.push_back( std::move(entry) );
  }

  this->itr_pos_ = 0;
  this->num_actions_ = this->actions_.size();
}

GroupAction::~GroupAction()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_ACTION,
                      nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::unique_ptr<GroupAction::derived_type> GroupAction::clone() const
{
  return( std::unique_ptr<self_type>( new self_type(*this) ) );
}

IActionObject::FrameState
GroupAction::update(real32 delta_time, uint32 direction)
{
  std::string action_id = "action";

  // Program flow is structured to never call back here after the actions are
  // finished -- this serves only as a reminder to the intended flow.
  if( this->status_ == FrameState::COMPLETED ) {
    NOM_ASSERT_INVALID_PATH();
    return this->status_;
  }

  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    IActionObject* action = (*itr).action.get();
    FrameState action_status = FrameState::COMPLETED;

    if( action != nullptr ) {

      if( action->name() != "" ) {
        action_id = action->name();
      }

      if( direction == FrameStateDirection::NEXT_FRAME ) {
        action_status = action->next_frame(delta_time);
      } else {
        action_status = action->prev_frame(delta_time);
      }
    }

    if( action_status == FrameState::COMPLETED ) {

      if( (*itr).status != FrameState::COMPLETED ) {

        (*itr).status = FrameState::COMPLETED;
        ++this->itr_pos_;

        NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                        action_id, "has finished at",
                        Timer::to_seconds( nom::ticks() ),
                        "[", this->itr_pos_, "/", this->num_actions_, "]",
                        "[action_id]:", this->name() );
      }

    }

    if( this->itr_pos_ == this->num_actions_ ) {
      NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION, DEBUG_CLASS_NAME,
                      "Finished at:", Timer::to_seconds( nom::ticks() ),
                      "[itr_pos]:", itr_pos_ );
      this->status_ = FrameState::COMPLETED;
      return this->status_;
    } else {
      this->status_ = FrameState::PLAYING;
    }

  }

  return this->status_;
}

IActionObject::FrameState GroupAction::next_frame(real32 delta_time)
{
  return this->update(delta_time, FrameStateDirection::NEXT_FRAME);
}

IActionObject::FrameState GroupAction::prev_frame(real32 delta_time)
{
  return this->update(delta_time, FrameStateDirection::PREV_FRAME);
}

void GroupAction::pause(real32 delta_time)
{
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    IActionObject* action = (*itr).action.get();
    if( action != nullptr ) {
      action->pause(delta_time);
    }
  } // end for loop
}

void GroupAction::resume(real32 delta_time)
{
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    IActionObject* action = (*itr).action.get();
    if( action != nullptr ) {
      action->resume(delta_time);
    }
  } // end for loop
}

void GroupAction::rewind(real32 delta_time)
{
  this->itr_pos_ = 0;

  // NOTE: Since this object type does not handle its frame state based on time
  // intervals like the other actions do, we must explicitly reset it here for
  // looping actions to work correctly.
  this->status_ = FrameState::PLAYING;

  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    (*itr).status = FrameState::PLAYING;
    IActionObject* action = (*itr).action.get();
    if( action != nullptr ) {
      action->rewind(delta_time);
    }
  } // end for loop
}

void GroupAction::release()
{
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    IActionObject* action = (*itr).action.get();
    if( action != nullptr ) {
      action->release();
    }
  } // end for loop
}

void GroupAction::set_speed(real32 speed)
{
  IActionObject::set_speed(speed);

  // Propagate the speed modifier for our children
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    IActionObject* action = (*itr).action.get();
    if( action != nullptr ) {
      action->set_speed(speed);
    }
  } // end for loop
}

void
GroupAction::set_timing_curve(const IActionObject::timing_curve_func& mode)
{
  IActionObject::set_timing_curve(mode);

  // Propagate the timing mode for our children
  for( auto itr = this->actions_.begin(); itr != this->actions_.end(); ++itr ) {

    IActionObject* action = (*itr).action.get();
    if( action != nullptr ) {
      action->set_timing_curve(mode);
    }
  } // end for loop
}

// Private scope

const GroupAction::container_type& GroupAction::actions() const
{
  return this->actions_;
}

} // namespace nom

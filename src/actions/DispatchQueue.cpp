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
#include "nomlib/actions/DispatchQueue.hpp"

// Forward declarations
#include "nomlib/actions/IActionObject.hpp"
#include "nomlib/actions/ActionPlayer.hpp"

namespace nom {

/// Internal representation of the action queue
///
/// \see nom::DispatchQueue::enqueue_action
struct DispatchEnqueue
{
  /// \brief The stored action object.
  std::shared_ptr<IActionObject> action;

  /// \brief An optional delegate to call upon when the action is completed.
  action_callback on_completion_callback;

  /// \brief Diagnostic iterator counter.
  nom::size_type action_pos = 0;
};

DispatchQueue::DispatchQueue()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ACTION, NOM_LOG_PRIORITY_VERBOSE);
}

DispatchQueue::~DispatchQueue()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_ACTION, NOM_LOG_PRIORITY_VERBOSE);
}

nom::size_type DispatchQueue::num_actions() const
{
  return( this->actions_.size() );
}

bool DispatchQueue::
enqueue_action( const std::shared_ptr<IActionObject>& action,
                const action_callback& completion_func )
{
  DispatchEnqueue enqueue;

  enqueue.action = action;
  NOM_ASSERT(enqueue.action != nullptr);
  if( enqueue.action == nullptr ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not enqueue the action; action was NULL." );
    return false;
  }

  enqueue.on_completion_callback = completion_func;

  auto queue_ptr =
    std::make_shared<DispatchEnqueue>(enqueue);
  this->actions_.emplace_back(queue_ptr);

  return true;
}

bool DispatchQueue::update(uint32 player_state, real32 delta_time)
{
  if( this->actions_.empty() == true ) {
    // Done updating; nothing to update
    return false;
  }

  auto itr = this->actions_.begin();
  NOM_ASSERT(itr != this->actions_.end() );

  auto action = (*itr)->action;
  if( action == nullptr ) {
    // Done updating; nothing to update
    return false;
  }

  std::string action_id = action->name();
  action_callback completion_func =
    (*itr)->on_completion_callback;

  // Diagnostics
  nom::size_type action_pos = (*itr)->action_pos;
  nom::size_type num_actions = this->actions_.size();
  NOM_ASSERT(action_pos >= 0);
  NOM_ASSERT(action_pos <= num_actions);

  IActionObject::FrameState action_status =
    IActionObject::FrameState::COMPLETED;

  action_status = action->next_frame(delta_time);

  // Handle the current animation with respect to the player's state
  if( action_status != IActionObject::FrameState::COMPLETED ) {

    if( player_state == ActionPlayer::State::PAUSED ) {
      action->pause(delta_time);
    } else if( player_state == ActionPlayer::State::STOPPED ) {
      action->rewind(delta_time);
    } else {
      action->resume(delta_time);
    } // ActionPlayer::State::RUNNING
  } // end if status != COMPLETED

  // EOF -- handle internal clean up
  if( action_status == IActionObject::FrameState::COMPLETED ) {

    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_ACTION_QUEUE, "DispatchQueue [erasing]:",
                    "[", action_pos + 1, "/", num_actions, "]",
                    "[id]:", action_id );

    ++(*itr)->action_pos;

    // Goodbye cruel world!
    this->actions_.erase(itr);

    // Holla back
    if( completion_func != nullptr ) {
      completion_func.operator()();
    }

    // Update iteration is finished
    return true;

  } // end if itr == FrameState::COMPLETED

  // Update iteration is **not** finished
  return false;
}

} // namespace nom

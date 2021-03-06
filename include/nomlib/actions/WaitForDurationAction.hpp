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
#ifndef NOMLIB_ACTIONS_WAIT_FOR_DURATION_ACTION_HPP
#define NOMLIB_ACTIONS_WAIT_FOR_DURATION_ACTION_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/actions/IActionObject.hpp"

namespace nom {

/// \brief Create an action that idles for a specified period of time
class WaitForDurationAction: public virtual IActionObject
{
  public:
    /// \brief Allow access into our private parts for unit testing.
    friend class ActionTest;

    typedef WaitForDurationAction self_type;
    typedef IActionObject derived_type;

    /// \brief Create an action that idles for a specified period of time.
    ///
    /// \param seconds The duration of the idle.
    WaitForDurationAction(real32 seconds);

    virtual ~WaitForDurationAction();

    virtual std::unique_ptr<IActionObject> clone() const override;

    virtual IActionObject::FrameState next_frame(real32 delta_time) override;

    virtual IActionObject::FrameState prev_frame(real32 delta_time) override;

    virtual void pause(real32 delta_time) override;

    virtual void resume(real32 delta_time) override;

    virtual void rewind(real32 delta_time) override;

    virtual void release() override;

  private:
    static const char* DEBUG_CLASS_NAME;
};

} // namespace nom

#endif // include guard defined

/// \class nom::WaitForDurationAction
/// \ingroup actions
///
/// \brief When the action executes, the action waits for the specified amount
/// of time, then ends. This is typically used as part of a sequence of actions
/// to insert a delay between two other actions. You might also use it in
/// conjunction with nom::ActionPlayer::run_action(action, completion_func)
/// to trigger logic that needs to run at a later time.
///
/// \remarks This action is not reversible; the reverse of this action is
/// the same action.
///

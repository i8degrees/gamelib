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
#ifndef NOMLIB_GRAPHICS_GROUP_ACTION_HPP
#define NOMLIB_GRAPHICS_GROUP_ACTION_HPP

#include <memory>
#include <deque>

#include "nomlib/config.hpp"
#include "nomlib/graphics/IActionObject.hpp"

namespace nom {

/// \brief Proxy action for running actions together (in parallel)
class GroupAction: public virtual IActionObject
{
  public:
    /// \brief Allow access into our private parts for unit testing.
    friend class AnimationTest;

    friend class RepeatForAction;
    friend class RepeatForeverAction;

    typedef GroupAction self_type;
    typedef IActionObject derived_type;

    /// \brief Default constructor.
    GroupAction(  const action_list& actions,
                  const std::string& name = "" );

    /// \brief Destructor.
    virtual ~GroupAction();

    virtual std::unique_ptr<derived_type> clone() const override;

    virtual IActionObject::FrameState next_frame(real32 delta_time) override;
    virtual IActionObject::FrameState prev_frame(real32 delta_time) override;

    virtual void pause(real32 delta_time) override;

    /// \brief Resume logic for the animation object.
    ///
    /// \remarks Reserved for future implementation.
    virtual void resume(real32 delta_time) override;

    virtual void rewind(real32 delta_time) override;

    virtual void release() override;

    /// \brief Set the action's speed modifier.
    ///
    /// \remarks The speed modifier of every action is modified.
    virtual void set_speed(real32 speed) override;

    /// \brief Set the action's timing mode.
    ///
    /// \remarks The timing mode of every action is modified.
    virtual
    void set_timing_curve(const IActionObject::timing_curve_func& mode) override;

  private:
    typedef std::vector<std::shared_ptr<IActionObject>> container_type;

    const container_type& actions() const;

    /// \brief The enqueued actions.
    ///
    /// \remarks This is not a queue container because we need to erase actions
    /// based on the order in which they complete.
    container_type actions_;

    /// \brief Iteration counter.
    nom::size_type itr_pos_;

    /// \brief Total number of actions at the time of construction.
    nom::size_type num_actions_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::GroupAction
/// \ingroup graphics
///
/// \brief This is perhaps the most fundamental animation type. This action
/// acts on behalf of other action(s) as a proxy, grouping actions together so
/// that every action within its container is updated **simultaneously** --
/// in parallel, until each action has completed.
///
/// \remarks This action is not reversible, but the action(s) contained
/// within may be. Consult the documentation for each action type in question.
///
/// \note Although the grouping of objects came first as a necessity of
/// convenience, the particular fashion in which things are updated is
/// paramount for the proper time step increments to occur. In short, actions
/// that are "related" to one another **must** share a common time step, else
/// they fall out of sync by one or more frames.
///
/// \see nom::SequenceAction
///

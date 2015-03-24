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
#ifndef NOMLIB_ACTIONS_SPRITE_BATCH_ACTION_HPP
#define NOMLIB_ACTIONS_SPRITE_BATCH_ACTION_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/actions/IActionObject.hpp"

namespace nom {

// Forward declarations
class SpriteBatch;

/// \brief Animate a sprite using a sprite sheet
class SpriteBatchAction: public virtual IActionObject
{
  public:
    /// \brief Allow access into our private parts for unit testing.
    friend class ActionTest;

    typedef SpriteBatchAction self_type;
    typedef IActionObject derived_type;

    /// \brief Animate a sprite using a sprite sheet.
    ///
    /// \param drawable A valid nom::SpriteBatch instance with a pre-loaded
    /// nom::SpriteSheet filled with the frames to iterate through.
    ///
    /// \param frame_interval_seconds The duration (in seconds) that each
    /// texture is displayed.
    ///
    /// \see nom::SpriteBatch, nom::SpriteSheet
    SpriteBatchAction(  const std::shared_ptr<SpriteBatch>& drawable,
                        real32 frame_interval_seconds );

    virtual ~SpriteBatchAction();

    virtual std::unique_ptr<IActionObject> clone() const override;

    virtual IActionObject::FrameState next_frame(real32 delta_time) override;

    virtual IActionObject::FrameState prev_frame(real32 delta_time) override;

    virtual void pause(real32 delta_time) override;

    virtual void resume(real32 delta_time) override;

    virtual void rewind(real32 delta_time) override;

    virtual void release() override;

  private:
    static const char* DEBUG_CLASS_NAME;

    IActionObject::FrameState
    update(real32 t, real32 b, real32 c, real32 d);

    void first_frame(real32 delta_time);
    void last_frame(real32 delta_time);

    /// \brief The initial sprite sheet frame to animate from.
    uint32 initial_frame_;

    /// \brief The total number of sprite sheet frames to animate.
    real32 total_displacement_;

    /// \brief The drawable sprite.
    std::shared_ptr<SpriteBatch> drawable_;

    /// \brief The delay (in seconds) before displaying the next texture.
    real32 frame_interval_;
    real32 last_delta_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::SpriteBatchAction
/// \ingroup actions
///
/// \brief ...
///
/// \remarks This action is reversible; the texture frames are displayed in the
/// inverse order. For example, if the texture frames are
/// {"tex0", "tex1", "tex2"}, the reversed order would be
/// {"tex2", "tex1", "tex0"}.
///

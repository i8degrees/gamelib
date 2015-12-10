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
#ifndef NOMLIB_AUDIO_NULL_SOUND_BUFFER_HPP
#define NOMLIB_AUDIO_NULL_SOUND_BUFFER_HPP

#include <string>

#include "nomlib/config.hpp"
#include "nomlib/audio/ISoundBuffer.hpp"

namespace nom {

class NullSoundBuffer: public ISoundBuffer
{
  public:
    NullSoundBuffer();
    virtual ~NullSoundBuffer();

    uint32 buffer_id() const override;
    const std::vector<int16>& samples() const override;
    int64 duration() const override;

    uint32 channel_count() const override;
    uint32 channel_format() const override;
    uint32 sample_rate() const override;
    nom::size_type sample_count() const override;
    nom::size_type audio_bytes() const override;
    void set_buffer(const std::vector<int16>& samples) override;

    bool load_file(const std::string& filename) override;

  private:
    std::vector<int16> samples_;
};

} // namespace nom

#endif // include guard defined

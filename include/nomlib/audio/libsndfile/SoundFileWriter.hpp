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
#ifndef NOMLIB_AUDIO_LIBSNDFILE_SOUND_FILE_WRITER_HPP
#define NOMLIB_AUDIO_LIBSNDFILE_SOUND_FILE_WRITER_HPP

#include <string>

#include "nomlib/config.hpp"
#include "nomlib/audio/SoundFile.hpp"
#include "nomlib/audio/ISoundFileWriter.hpp"

// Forward declarations
struct SNDFILE_tag;
struct SF_INFO;

namespace nom {
namespace audio {

// TODO(jeff): Rename to RawSoundFileWriter or so
class SoundFileWriter: public ISoundFileWriter
{
  public:
    SoundFileWriter();

    virtual ~SoundFileWriter();

    virtual bool valid() const override;

    virtual bool open(const std::string& filename, SoundInfo& info) override;

    virtual
    int64 write(void* data, nom::size_type byte_size) override;

    virtual int64 seek(int64 offset, SoundSeek dir) override;

    virtual void close() override;

  private:
    SoundInfo parse_header(SF_INFO& metadata);
    static const char* parse_tags(SNDFILE_tag* fp, uint32 sound_tag);

    /// \brief A third-party file descriptor whose owned by the end-user.
    ///
    /// \see libsndfile
    SNDFILE_tag* fp_ = nullptr;
};

} // namespace audio
} // namespace nom

#endif // inclued guard defined

/******************************************************************************

    OpenAL Audio Device

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_AL_AUDIO_DEVICE_HEADERS
#define NOMLIB_AL_AUDIO_DEVICE_HEADERS

#include <iostream>
#include <memory>

#include "OpenAL.hpp"

namespace nom {
  namespace OpenAL {

class AudioDevice
{
  public:
    AudioDevice ( void );
    ~AudioDevice ( void );

    /// Obtain the OpenAL audio device
    std::shared_ptr<ALCdevice> getAudioDevice ( void ) const;

    const std::string getDeviceName ( void ) const;

  private:
    /// Audio device handle
    std::shared_ptr<ALCdevice> audio_device;
    /// Audio device context
    std::shared_ptr<ALCcontext> audio_context;
    /// device name
    const ALCchar *device_name;
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_AUDIO_DEVICE_HEADERS defined

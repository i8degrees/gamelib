/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#include "nomlib/system/OSX/DialogMessageBox.hpp"

namespace nom {

int32 OSX_DialogMessageBox  ( const std::string& header, const std::string& message,
                              int32 messagebox_type
                            )
{
  CFOptionFlags result; //result code from the message box
  CFStringRef header_ref = CFStringCreateWithCString (  nullptr, header.c_str(),
                                                        strlen ( header.c_str() )
                                                      );
  CFStringRef message_ref = CFStringCreateWithCString ( nullptr, message.c_str(),
                                                        strlen ( message.c_str() )
                                                      );

  CFUserNotificationDisplayAlert  (
                                    0, // no timeout
                                    // type of notification
                                    kCFUserNotificationNoteAlertLevel,
                                    nullptr, // icon url
                                    nullptr, // not used
                                    nullptr, // localization of strings
                                    header_ref, // header text
                                    message_ref, // message text
                                    nullptr, // default "ok" text in button
                                    nullptr, // alternate button title
                                    nullptr, // other button title
                                    &result // response flags
                                  );

  CFRelease ( header_ref );
  CFRelease ( message_ref );

  if( result == kCFUserNotificationDefaultResponse )
    return ID_OK;
  else
    return -1; // err
}


} // namespace nom

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
#include "nomlib/system/unix/DialogMessageBox.hpp"

namespace nom {

int32 GTK_DialogMessageBox  ( const std::string& header, const std::string& message,
                              int32 messagebox_type
                            )
{
  nom::int32 result = -1;
  nom::uint32 flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
  GtkWidget* dialog;

  gtk_init ( nullptr, nullptr );

  dialog = gtk_message_dialog_new ( nullptr, flags,
                                    GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                    header.c_str(), message.c_str()
                                  );

  result = gtk_dialog_run ( GTK_DIALOG (dialog) );

  gtk_widget_destroy ( dialog );

  switch ( result )
  {
    case -1:
    default: return result; break; // Err
    case GTK_RESPONSE_OK: return ID_OK; break;
    case GTK_RESPONSE_CANCEL: return ID_CANCEL; break;
  }
}


} // namespace nom

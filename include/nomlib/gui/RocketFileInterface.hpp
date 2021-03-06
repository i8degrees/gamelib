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
#ifndef NOMLIB_GUI_ROCKET_FILE_INTERFACE_HPP
#define NOMLIB_GUI_ROCKET_FILE_INTERFACE_HPP

#include <string>

#include "nomlib/config.hpp"

#include <Rocket/Core/String.h>
#include <Rocket/Core/FileInterface.h>

namespace nom {

/* Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

/**
  Rocket file interface for the shell examples.
  @author Lloyd Weehuizen
 */

/// \brief Filesystem interfacing bridge between libRocket and nomlib
class RocketFileInterface: public Rocket::Core::FileInterface
{
  public:
    RocketFileInterface( const std::string& root );

    virtual ~RocketFileInterface();

    /// \brief Implements Rocket::Core::FileInterface::Release.
    ///
    /// \remarks Called when this file interface is released.
    virtual void Release();

    /// Opens a file.
    virtual Rocket::Core::FileHandle Open(const Rocket::Core::String& path);

    /// Closes a previously opened file.
    virtual void Close(Rocket::Core::FileHandle file);

    /// Reads data from a previously opened file.
    virtual nom::size_type Read(void* buffer, nom::size_type size, Rocket::Core::FileHandle file);

    /// Seeks to a point in a previously opened file.
    virtual bool Seek(Rocket::Core::FileHandle file, long offset, int origin);

    /// Returns the current position of the file pointer.
    virtual nom::size_type Tell(Rocket::Core::FileHandle file);

    /// \brief Get the root directory path.
    ///
    /// \returns A string containing the directory path root that this object
    /// was initialized with.
    std::string root() const;

  private:
    Rocket::Core::String root_;
};

} // namespace nom

#endif // include guard defined

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
#ifndef NOMLIB_IFILE_HEADERS
#define NOMLIB_IFILE_HEADERS

#include <iostream>
#include <string>
#include <cstring>

#include <nomlib/config.hpp>
#include <nomlib/system/Logger.hpp>

namespace nom {

/// \brief Abstract interface class for Platform-agnostic file access
class IFile
{
  public:
    IFile ( void ) {}
    virtual ~IFile ( void ) {}

    /// Obtain the input file's mime type; uses libmagic to determine said type
    ///
    /// Returns a string in the same format as the following terminal command:
    ///
    /// $ file -b --mime-type <file>
    ///
    /// Returns a null terminated string on err
    virtual const std::string mime ( const std::string& file ) = 0;

    /// Returns the file extension of the input file path
    ///
    /// Returns a null terminated string on err
    ///
    virtual const std::string extension ( const std::string& file ) = 0;

    /// Uses stat(2) to determine input file size (in bytes)
    ///
    /// -1 on err
    virtual int32 size ( const std::string& file_path ) = 0;

    /// Uses stat(2) to determine if the input file exists
    ///
    virtual bool exists ( const std::string& file_path ) = 0;

    /// dirname(3) wrapper
    ///
    /// Extract the directory portion of a pathname
    ///
    /// dir_path is arbitrarily limited to 1024 characters.
    virtual const std::string path ( const std::string& dir_path ) = 0;

    /// getcwd(3) wrapper
    ///
    /// Returned path is arbitrarily limited to 1024 characters.
    virtual const std::string currentPath ( void ) = 0;

    /// chdir(2) wrapper
    virtual void setPath ( const std::string& path ) = 0;
};


} // namespace nom

#endif // include guard

/// \class nom::IFile
///

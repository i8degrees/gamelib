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
#ifndef NOMLIB_FILE_HEADERS
#define NOMLIB_FILE_HEADERS

#include <iostream>
#include <string>
#include <cstring>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/system/IFile.hpp"

#if defined ( NOM_PLATFORM_OSX ) || defined ( NOM_PLATFORM_POSIX )

  #include "nomlib/system/unix/UnixFile.hpp"

#elif defined ( NOM_PLATFORM_WINDOWS )

  #include "nomlib/system/windows/WinFile.hpp"

#endif

namespace nom {

/// \brief Platform-agnostic file system interface
class File
{
  public:
    File ( void );
    ~File ( void );

    /// Re-implements nom::IFile::extension
    const std::string extension ( const std::string& file );

    /// Re-implements nom::IFile::size
    int32 size ( const std::string& file_path );

    /// \brief Test for the existence of a directory.
    bool is_dir( const std::string& file_path );

    bool is_file( const std::string& file_path );

    /// Re-implements nom::IFile::exists
    bool exists( const std::string& file_path );

    /// \brief Platform-specific implementation of IFile::path.
    ///
    /// \see UnixFile::path, WinFile::path.
    const std::string path( const std::string& dir_path );

    /// Re-implements nom::IFile::currentPath
    std::string currentPath( void ) const;

    /// Re-implements nom::IFile::set_path
    bool set_path ( const std::string& path );

    /// Re-implements nom::IFile::basename
    const std::string basename ( const std::string& filename );

    std::vector<std::string> read_dir( const std::string& dir_path );

    const std::string resource_path( const std::string& identifier = "\0" );

    const std::string user_documents_path( void );

    const std::string user_app_support_path( void );

    const std::string user_home_path( void );

    const std::string system_path( void );

    /// \see UnixFile::mkdir, WinFile::mkdir.
    bool mkdir( const std::string& path );

    /// \see UnixFile::recursive_mkdir, WinFile::recursive_mkdir.
    bool recursive_mkdir( const std::string& path );

    /// \see UnixFile::rmdir, WinFile::rmdir.
    bool rmdir( const std::string& path );

    /// \see UnixFile::mkfile, WinFile::mkfile.
    bool mkfile( const std::string& path );

    /// \see UnixFile::env, WinFile::env.
    std::string env( const std::string& path );

  private:
    std::unique_ptr<IFile> file;
};

namespace priv {

/// \brief The root directory path.
extern std::string file_root_;

} // namespace priv

/// \brief Get the root directory path.
///
/// \returns A string containing the directory path root.
///
/// \see nom::set_file_root.
std::string file_root();

/// \brief Set the root directory path.
///
/// \param root The absolute directory path, ending with a slash.
///
/// \remarks This path can be used to establish a relative directory path to
/// prepend onto a file name to form a complete, absolute path.
///
/// \see nom::BMFont::build
void set_file_root(const std::string& root);

} // namespace nom

#endif // include guard

/// \class nom::File
///
/// \todo Declare methods static
///

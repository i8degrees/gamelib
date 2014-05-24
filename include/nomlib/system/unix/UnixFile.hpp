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
#ifndef NOMLIB_UNIX_FILE_HEADERS
#define NOMLIB_UNIX_FILE_HEADERS

#include <iostream>
#include <string>
#include <cstring>
#include <vector>

#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#include <dirent.h>

#include "nomlib/config.hpp"
#include "nomlib/system/IFile.hpp"

#if defined( NOM_PLATFORM_OSX )
  #include <CoreServices/CoreServices.h>
  #include <CoreFoundation/CoreFoundation.h>
#endif

namespace nom {

/// \brief Platform-specific interface for file based access in Unix
///
/// \see http://www.gnu.org/software/libc/manual/html_node/File-System-Interface.html#File-System-Interface
class UnixFile: public IFile
{
  public:
    UnixFile ( void );
    ~UnixFile ( void );

    /// Implements nom::IFile::extension
    ///
    /// \remarks See nom::IFile::extension
    const std::string extension ( const std::string& file );

    /// Implements nom::IFile::size
    ///
    /// \remarks See nom::IFile::size
    int32 size ( const std::string& file_path );

    /// \brief Test for the existence of a directory.
    ///
    /// \see http://stackoverflow.com/questions/16312872/cannot-distinguish-directory-from-file-with-stat
    bool is_dir( const std::string& file_path );

    /// Implements nom::IFile::exists
    ///
    /// \remarks See nom::IFile::exists
    bool exists ( const std::string& file_path );

    /// Implements nom::IFile::path
    ///
    /// \remarks See nom::IFile::path
    const std::string path ( const std::string& dir_path );

    /// Implements nom::IFile::currentPath
    ///
    /// \remarks See nom::IFile::currentPath
    const std::string currentPath ( void );

    /// Implements nom::IFile::set_path
    ///
    /// \remarks See nom::IFile::set_path
    bool set_path ( const std::string& path );

    /// Implements nom::IFile::basename
    ///
    /// \remarks See nom::IFile::basename
    const std::string basename ( const std::string& filename );

    /// \brief Get a list of file entries within a directory.
    ///
    /// \remarks The directory entries '.' and '..' are removed from the file
    /// listing.
    ///
    /// \todo Implement optional directory recursion.
    ///
    /// \note Source reference: http://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html#Simple-Directory-Lister.
    std::vector<std::string> read_dir( const std::string& dir_path );

    /// \brief Get the path to the Resources folder within a bundle.
    ///
    /// \remarks If no identifier argument is passed, it is assumed you are
    /// looking for the main application bundle's Resources path, otherwise the
    /// identifier is used to locate a particular bundle from within another
    /// bundle.
    ///
    /// The bundle's identifier can be found within the Info.plist file of said
    /// bundle, and typically is in reverse DNS notation. The string might look
    /// something like this:
    ///
    ///   org.dev.nomlib
    ///
    ///   or, as per the official API documentation,
    ///
    ///   com.apple.Finder.MyGetInfoPlugIn
    ///
    /// The one restriction you have in using this function call is that the bundle
    /// whose identifier you have passed must already be loaded into the application
    /// bundle, or else this function call will fail.
    ///
    /// \returns A null-terminated string on err.
    const std::string resource_path( const std::string& identifier = "\0" );

    /// \brief Get the path to the logged in user's Documents folder.
    ///
    ///     $HOME/Documents/
    ///
    /// \remarks These are standard folders that may be used for saving user
    /// data under.
    ///
    /// \todo FSFindFolder and FSRefMakePath are deprecated in OS X v10.8+.
    const std::string user_documents_path( void );

    /// \brief Get the path to the logged in user's Application Support folder.
    ///
    ///     $HOME/Library/Application\ Support/
    ///
    /// \remarks These are standard folders that may be used for saving user
    /// data under.
    ///
    /// \todo FSFindFolder and FSRefMakePath are deprecated in OS X v10.8+.
    const std::string user_app_support_path( void );
};


} // namespace nom

#endif // include guard

/// \class nom::UnixFile
///

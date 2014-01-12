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
#include "nomlib/system/File.hpp"

namespace nom {

File::File ( void )
{
  #if defined (NOM_PLATFORM_WINDOWS) // Use Windows APIs
    this->file = std::shared_ptr<IFile> ( new WinFile() );
  #else // Assume POSIX compatibility; use POSIX / BSD & GNU APIs
    this->file = std::shared_ptr<IFile> ( new UnixFile() );
  #endif
}

File::~File ( void ) {}

const std::string File::extension ( const std::string& file )
{
  return this->file->extension ( file );
}

int32 File::size ( const std::string& file_path )
{
  return this->file->size ( file_path );
}

bool File::exists ( const std::string& file_path )
{
  return this->file->exists ( file_path );
}

const std::string File::path ( const std::string& dir_path )
{
  return this->file->path ( dir_path );
}

const std::string File::currentPath ( void )
{
  return this->file->currentPath();
}

bool File::set_path ( const std::string& path )
{
  return this->file->set_path(path);
}

const std::string File::basename ( const std::string& filename )
{
  return this->file->basename(filename);
}

} // namespace nom

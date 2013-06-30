/******************************************************************************
    OSXFs.cpp

  (OSX) Filesystem Interface

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "OSXFs.h"

nom::OSXFs::OSXFs ( void )
{
  #ifdef DEBUG_OSXFS_OBJ
    std::cout << "nom::OSXFs::OSXFs (): " << "Hello, world!" << std::endl << std::endl;
  #endif
}

nom::OSXFs::~OSXFs ( void )
{
  #ifdef DEBUG_OSXFS_OBJ
    std::cout << "nom::OSXFs::~OSXFs (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif
}

// Not sure how else to do this but in straight C
const char* nom::OSXFs::getDirName ( char *path )
{
  // ERR checking

  return dirname ( path );
}

const std::string nom::OSXFs::getWorkingDir ( void )
{
  char path[PATH_MAX];

  getcwd ( path, PATH_MAX );

  // ERR checking

  std::string cwd ( path );

  return cwd;
}

void nom::OSXFs::setWorkingDir ( std::string path )
{
  chdir ( path.c_str() );

  // ERR checking
}

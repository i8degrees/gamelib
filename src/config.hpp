/******************************************************************************

    Configuration file for nomlib library

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_CONFIG_HEADERS
#define NOMLIB_CONFIG_HEADERS

#include <iostream>
#include <cassert>

// nomlib version
#include "version.hpp"

#include "types.hpp"
#include "sys/Logger.hpp"

// Identification the operating system
#if defined ( _WIN32) || defined ( __WIN32__ )
  #define NOMLIB_SYSTEM_WINDOWS
#elif defined ( linux ) || defined ( __linux )
  #define NOMLIB_SYSTEM_LINUX
#elif defined ( __APPLE__ ) || defined ( MACOSX ) || defined ( macintosh ) || defined ( Macintosh )
  #define NOMLIB_SYSTEM_OSX
#else
  #warning This operating system is not officially supported by nomlib
#endif

// Function names and preferably also its type signature
#if defined ( _MSC_VER ) // MSVC++
  // TODO: Presumably the same as GNU's __PRETTY_FUNCTION__ ?
  //
  // SOURCE: http://msdn.microsoft.com/en-us/library/b0084kay(v=vs.80).aspx
  #define __func__ __FUNCSIG__
#else // We assume GNU v2+

  // The type signature is nice because this shows if the function calling type
  // is a virtual or not and even what arguments the function has
  #define __func__ __PRETTY_FUNCTION__
#endif

// nomlib debugging

// Standard debug level; logging of info, warnings & errors
#define NOMLIB_DEBUG

// Internal development; logging of class object construction and destruction
#define NOMLIB_DEBUG_ALL

// Pretty print C macros
#ifdef NOMLIB_DEBUG
  // If debugging is turned on, we log all warnings, errors & info
  #define NOMLIB_LOG(message) \
    ( nom::Logger::info ( message ) )
  #define NOMLIB_LOG_ERR(message) \
    ( nom::Logger::err ( __FILE__, __LINE__, message ) )
  #define NOMLIB_ASSERT(expression) \
    ( assert (expression) )
#else // We do not add any overhead
  #define NOMLIB_LOG(message)
  #define NOMLIB_LOG_ERR(message)
  #define NOMLIB_ASSERT(expression)
#endif

#ifdef NOMLIB_DEBUG_ALL
  // If all debugging is turned on, we show class construction and destruction
  #define NOMLIB_LOG_INFO \
    ( nom::Logger::info ( __func__ ) )
#else // We do not add any overhead
  #define NOMLIB_LOG_INFO
#endif

#endif // NOMLIB_CONFIG_HEADERS defined

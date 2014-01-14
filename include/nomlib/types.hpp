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
#ifndef NOMLIB_TYPES_HEADERS
#define NOMLIB_TYPES_HEADERS

#include <cstdint>

#include "platforms.hpp"

// Portable fixed-size data types derive from stdint.h
namespace nom {

// 8-bit integer types
typedef int8_t int8;
typedef uint8_t uint8;

// 16-bit integer types
typedef int16_t int16;
typedef uint16_t uint16;

// 32-bit integer types
typedef int32_t int32;
typedef uint32_t uint32;

// 64-bit integer types
typedef signed long long int int64;
typedef unsigned long long int uint64;

// Additional integer type definitions
#if defined (NOM_PLATFORM_ARCH_X86_64)
  typedef unsigned long ulong;
#else // Blindly assume 32-bit arch
  typedef long long ulong;
#endif

typedef unsigned char uchar;
typedef signed int sint;
typedef unsigned int uint;
typedef std::size_t size_t;
typedef int boolean;

typedef sint* sint_ptr;
typedef uint* uint_ptr;

typedef int32_t* int32_ptr;
typedef uint32_t* uint32_ptr;

typedef void* void_ptr;

} // namespace nom

/// Ensure our data types have the right sizes using C++11 compile-time asserts.
static_assert ( sizeof ( nom::uint8 ) == 1, "nom::uint8" );
static_assert ( sizeof ( nom::int8 ) == 1, "nom::int8" );

static_assert ( sizeof ( nom::uint16 ) == 2, "nom::uint16" );
static_assert ( sizeof ( nom::int16 ) == 2, "nom::int16" );

static_assert ( sizeof ( nom::uint32 ) == 4, "nom::uint32" );
static_assert ( sizeof ( nom::int32 ) == 4, "nom::int32" );

static_assert ( sizeof ( nom::uint64 ) == 8, "nom::uint64" );
static_assert ( sizeof ( nom::int64 ) == 8, "nom::int64" );

static_assert ( sizeof ( nom::ulong ) == 8, "nom::ulong" );

static_assert ( sizeof ( nom::uchar ) == 1, "nom::uchar" );

#if defined(NOM_PLATFORM_ARCH_X86_64)
  static_assert ( sizeof ( nom::size_t ) == ( sizeof(nom::uint64) ), "nom::size_t" );
  static_assert ( sizeof ( nom::int32_ptr ) == ( sizeof(long) ), "nom::int32_ptr" );
  static_assert ( sizeof ( nom::uint32_ptr ) == ( sizeof(nom::ulong) ), "nom::uint32_ptr" );
#elif defined(NOM_PLATFORM_ARCH_X86)
  static_assert ( sizeof ( nom::size_t ) == ( sizeof(nom::uint32) ), "nom::size_t" );
#else
  #if defined(NOM_COMPILER_MSVCPP)
    #pragma message ( "types.hpp: Unknown architecture; defined data types may be wrong." )
  #else // Assume GCC/llvm-clang
    #warning types.hpp: Unknown architecture; defined data types may be wrong.
  #endif
#endif

static_assert ( sizeof ( nom::boolean ) == ( sizeof(int) ), "nom::boolean" );

/// Additional type definitions

const nom::sint NOM_EXIT_FAILURE = 1; // EXIT_FAILURE from cstdlib headers
const nom::sint NOM_EXIT_SUCCESS = 0; // EXIT_SUCCESS from cstdlib headers

//#if defined(HAVE_SDL2)
const nom::sint SDL_SUCCESS = 0; // Non-error return value for SDL2 API
//#endif

#endif // include guard defined

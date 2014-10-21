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
#ifndef NOMLIB_CORE_CLOCK_HPP
#define NOMLIB_CORE_CLOCK_HPP

#include <iostream>
#include <ctime>
#include <locale>

#include <algorithm>  // std::max

#include "nomlib/config.hpp"

namespace nom {

const nom::size_type TIME_STRING_SIZE = 26;

/// \brief Get the current date & time.
///
/// \returns A std::string containing the formatted date and time stamp on
/// success, or a null-terminated string on err.
///
/// \param format Conversion specifiers that are compatible with std::strftime.
///
/// \note This method is platform-specific.
///
/// \see http://en.cppreference.com/w/cpp/chrono/c/strftime
/// \see http://msdn.microsoft.com/en-us/library/fe06s4ak(v=vs.71).aspx
std::string time( const std::string& format );

/// \brief Get the current date and time.
///
/// \returns A std::string containing the ISO 8601 date and time on success, or
/// a null-terminated string on err.
///
/// Sample output:
///
/// \code
/// 2014-06-02 02:52:42
/// \endcode
std::string timestamp( void );

/// \brief Get the current date and time, made friendly towards use in a file
/// name.
///
/// \returns A std::string containing the ISO 8601 date and time stamp on
/// success, or a null-terminated string on err.
///
/// Sample output:
///
/// \code
/// 2014-06-19_13-25-21
/// \endcode
std::string file_timestamp( void );

/// SDL helper function
///
/// Wrapper for SDL_GetTicks.
uint32 ticks( void );
std::string ticks_as_string( void );

/// SDL_Delay wrapper
///
/// Values below 10 milliseconds are clamped to 10
void sleep( uint32 milliseconds );

/// \brief Get the current value of the high resolution counter.
///
/// \returns The current counter value.
///
/// \remarks Typically used for profiling. The counter values are only
/// meaningful relative to each other. Differences between values can be
/// converted to times by using hires_frequency().
///
/// \note This is a function wrapper for SDL_GetPerformanceCounter.
/// \see https://wiki.libsdl.org/SDL_GetPerformanceCounter?highlight=%28%5CbCategoryTimer%5Cb%29%7C%28CategoryEnum%29%7C%28CategoryStruc%29
uint64 hires_counter();

/// \brief Get the count per second of the high resolution counter.
///
/// \returns A platform-specific count per second.
///
/// \note This is a function wrapper for SDL_GetPerformanceFrequency.
/// \see https://wiki.libsdl.org/SDL_GetPerformanceFrequency
uint64 hires_frequency();

} // namespace nom

#endif // include guard defined

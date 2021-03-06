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
#ifndef NOMLIB_MATH_HELPERS_HPP
#define NOMLIB_MATH_HELPERS_HPP

#include <random>
#include <cmath>

#include "nomlib/config.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"

namespace nom {

const double PI = 4.0 * atan ( 1.0 );

namespace priv {

extern std::random_device rd;
extern std::default_random_engine rd_generator;

} // namespace priv

/// \brief Initialize the default random engine.
///
/// \param seed The seed sequence value for the random number generator.
///
/// \remarks This function does not need to be called when the use of a known
/// seed source is not necessary.
///
/// \note This function should only be called once, and before using any of the
/// random number generators provided by nomlib.
///
/// \see nom::uniform_int_rand, nom::uniform_real_rand.
void init_rand(uint32 seed_seq);

/// \brief Generate a random number.
///
/// \return Random number between the specified start and end numbers.
///
/// \remarks This function should only be used with signed or unsigned integers.
template <typename T>
inline T uniform_int_rand(T start_range, T end_range)
{
  std::uniform_int_distribution<T> distribution(start_range, end_range);

  return distribution(priv::rd_generator);
}

/// \brief Generate a random number.
///
/// \return Random number between the specified start and end numbers.
///
/// \remarks This function should only be used with float or double numbers.
template <typename T>
inline T uniform_real_rand(T start_range, T end_range)
{
  std::uniform_real_distribution<T> distribution(start_range, end_range);

  return distribution(priv::rd_generator);
}

/// Rotates a given X & Y coordinate point along a given pivot axis
/// (rotation point) at the given angle (in degrees), clockwise.
const Point2d rotate_points ( float angle, float x, float y, float pivot_x, float pivot_y );

/// \brief Round a fractional value.
///
/// \param number The 32-bit floating-point number to round.
///
/// \remarks The number is round up when it is greater than 0.5 and rounded
/// down when the number is less than 0.5
template <typename T>
inline T round_float(real32 number)
{
  real32 ret = number < 0.0f ? ceilf(number - 0.5f) : floorf(number + 0.5f);
  return NOM_SCAST(T, ret);
}

/// \brief Round a fractional value down to the nearest integral number.
///
/// \param number The 32-bit floating-point number to round.
template <typename T>
inline T round_float_down(real32 number)
{
  real32 ret = floorf(number);
  return NOM_SCAST(T, ret);
}

/// \brief Round a fractional value up to the largest integral number.
///
/// \param number The 32-bit floating-point number to round.
template <typename T>
inline T round_float_up(real32 number)
{
  real32 ret = ceilf(number);
  return NOM_SCAST(T, ret);
}

/// \brief Round a fractional value.
///
/// \param number The 64-bit floating-point number to round.
///
/// \remarks The number is round up when it is greater than 0.5 and rounded
/// down when the number is less than 0.5.
template <typename T>
inline T round_double(real64 number)
{
  real64 ret = number < 0.0f ? ceil(number - 0.5f) : floor(number + 0.5f);
  return NOM_SCAST(T, ret);
}

template <typename T>
inline T truncate_float(real32 number)
{
  T ret = NOM_SCAST(T, number);
  return(ret);
}

template <typename T>
inline T absolute_float(real32 number)
{
  T ret = fabs(number);
  return(ret);
}

/// \brief Compare two numbers for the smaller of the two values.
///
/// \returns The smaller of the two given values.
template <typename T>
inline T minimum(const T& a, const T& b)
{
  T result = (b < a) ? b : a;
  return result;
}

/// \brief Compare two numbers for the larger of the two values.
///
/// \returns The larger of the two given values.
template <typename T>
inline T maximum(const T& a, const T& b)
{
  T result = (a < b) ? b : a;
  return result;
}

} // namespace nom

#endif // include guard defined

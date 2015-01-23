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
#include "nomlib/graphics/AnimationTimingModes.hpp"

// Private headers
#include "nomlib/math/math_helpers.hpp" // definition of PI

namespace nom {

// Ignore warnings generated by the use of the expression ```t /= d``` -- these
// warnings cause no known problems.
#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wunsequenced"

// ...Linear...

real32 Linear::ease_in(real32 t, real32 b, real32 c, real32 d)
{
  return c * (t /= d) + b;
}

real32 Linear::ease_out(real32 t, real32 b, real32 c, real32 d)
{
  return c * (t /= d) + b;
}

real32 Linear::ease_in_out(real32 t, real32 b, real32 c, real32 d)
{
  return c * (t /= d) + b;
}

// ...Quad...

real32 Quad::ease_in(real32 t, real32 b, real32 c, real32 d)
{
  return c * (t /= d) * t + b;
}

real32 Quad::ease_out(real32 t, real32 b, real32 c, real32 d)
{
  return -c * (t /= d) * (t - 2) + b;
}

real32 Quad::ease_in_out(real32 t, real32 b, real32 c, real32 d)
{
  // if( (t /= d / 2) < 1) {
  //   return( (c / 2) * (t * t) ) + b;
  // }

  // return -c / 2 * ( ((t - 2) * (--t)) - 1) + b;

  /*
  originally return -c/2 * (((t-2)*(--t)) - 1) + b;

  I've had to swap (--t)*(t-2) due to diffence in behaviour in
  pre-increment operators between java and c++, after hours
  of joy
  */

  // The above easing algorithm [1] does not return the proper frame step value
  // as per our unit test in AnimationTimingModesTest.QuadEaseInOut [2]. I
  // expected to see a return value of 300 at elapsed frame marker 1000, but
  // instead got a return value of 164.
  //
  // The easing algorithm below [3] works as expected.
  //
  // 1.https://github.com/jesusgollonet/ofpennereasing). The easing algorithms are the work of [Robbert Penner](http://www.robertpenner.com/easing/
  // 2. AnimationTimingModesTest.cpp
  // 3. http://gsgd.co.uk/sandbox/jquery/easing/jquery.easing.1.3.js
  if( (t /= d / 2) < 1 ) {
    return c / 2 * t * t + b;
  }

  return -c / 2 * ( (--t) * (t - 2) - 1) + b;
}

// ...Cubic...

real32 Cubic::ease_in(real32 t, real32 b, real32 c, real32 d)
{
  return c * (t /= d) * t * t + b;
}

real32 Cubic::ease_out(real32 t, real32 b, real32 c, real32 d)
{
  return c * ( (t = t / d - 1) * t * t + 1) + b;
}

real32 Cubic::ease_in_out(real32 t, real32 b, real32 c, real32 d)
{
  if( (t /= d / 2) < 1) {
    return c / 2 * t * t * t + b;
  }

  return c / 2 * ( (t -= 2) * t * t + 2) + b;
}

// ...Quart...

real32 Quart::ease_in(real32 t, real32 b, real32 c, real32 d)
{
  return c * (t /= d) * t * t * t + b;
}

real32 Quart::ease_out(real32 t, real32 b, real32 c, real32 d)
{
  return -c * ((t = t / d - 1) * t* t * t - 1) + b;
}

real32 Quart::ease_in_out(real32 t, real32 b, real32 c, real32 d)
{
  if( (t /= d / 2) < 1) {
    return c / 2 * t * t * t * t + b;
  }

  return -c / 2 * ((t -= 2) * t* t * t - 2) + b;
}

// ...Quint...

real32 Quint::ease_in(real32 t, real32 b, real32 c, real32 d)
{
  return c * (t /= d) * t * t * t * t + b;
}

real32 Quint::ease_out(real32 t, real32 b, real32 c, real32 d)
{
  return c * ( (t = t / d - 1) * t * t * t * t + 1) + b;
}

real32 Quint::ease_in_out(real32 t, real32 b, real32 c, real32 d)
{
  if( (t /= d / 2) < 1) {
    return c / 2 * t * t * t * t * t + b;
  }

  return c / 2 * ((t -= 2) * t * t * t * t + 2) + b;
}

// ...Back...

real32 Back::ease_in(real32 t, real32 b, real32 c, real32 d)
{
  float s = 1.70158f;
  float postFix = t /= d;

  return c * (postFix) * t * ( (s + 1) * t - s) + b;
}

real32 Back::ease_out(real32 t, real32 b, real32 c, real32 d)
{
  float s = 1.70158f;

  return c * ( (t = t / d - 1) * t * ( (s + 1) * t + s) + 1) + b;
}

real32 Back::ease_in_out(real32 t, real32 b, real32 c, real32 d)
{
  float s = 1.70158f;

  if( (t /= d / 2) < 1) {
    return c / 2 * (t * t * (((s *= (1.525f)) + 1) * t - s)) + b;
  }

  float postFix = t -= 2;

  return c / 2 * ( (postFix) * t * (((s *= (1.525f)) + 1) * t + s) + 2) + b;
}

// ...Bounce...

real32 Bounce::ease_in(real32 t, real32 b, real32 c, real32 d)
{
  return c - Bounce::ease_out(d - t, 0, c, d) + b;
}

real32 Bounce::ease_out(real32 t, real32 b, real32 c, real32 d)
{
  if( (t /= d) < (1 / 2.75f) ) {
    return c * (7.5625f * t * t) + b;
  } else if( t < (2 / 2.75f) ) {
    float postFix = t -= (1.5f / 2.75f);
    return c * (7.5625f * (postFix) * t + .75f) + b;
  } else if( t < (2.5 / 2.75) ) {
    float postFix = t -= (2.25f / 2.75f);
    return c * (7.5625f * (postFix) * t + .9375f) + b;
  } else {
    float postFix = t -= (2.625f / 2.75f);
    return c * (7.5625f * (postFix) * t + .984375f) + b;
  }
}

real32 Bounce::ease_in_out(real32 t, real32 b, real32 c, real32 d)
{
  if( t < d/2 ) {
    return Bounce::ease_in( t * 2, 0, c, d) * .5f + b;
  } else {
    return Bounce::ease_out( t * 2 - d, 0, c, d) * .5f + c * .5f + b;
  }
}

// ...Circ...

real32 Circ::ease_in(real32 t, real32 b, real32 c, real32 d)
{
  return -c * ( sqrt(1 - (t /= d) * t) - 1) + b;
}

real32 Circ::ease_out(real32 t, real32 b, real32 c, real32 d)
{
  return c * sqrt(1 - (t = t / d - 1) * t) + b;
}

real32 Circ::ease_in_out(real32 t, real32 b, real32 c, real32 d)
{
  if( (t /= d / 2) < 1) {
    return -c / 2 * (sqrt(1 - t * t) - 1) + b;
  }

  return c / 2 * (sqrt(1 - t * (t -= 2)) + 1) + b;
}

// ...Elastic...

real32 Elastic::ease_in(real32 t, real32 b, real32 c, real32 d)
{
  if(t == 0) {
    return b;
  }
  if( (t /= d) == 1) {
    return b + c;
  }

  float p = d * .3f;
  float a = c;
  float s = p / 4;
  // this is a fix, again, with post-increment operators
  float postFix = a * pow(2, 10 * (t -= 1));

  return -(postFix * sin( (t * d - s) * (2 * PI) / p ) ) + b;
}

real32 Elastic::ease_out(real32 t, real32 b, real32 c, real32 d)
{
  if(t == 0) {
    return b;
  }
  if( (t /= d) == 1) {
    return b + c;
  }

  float p = d * .3f;
  float a = c;
  float s = p / 4;

  return( a * pow(2, -10 * t) * sin( (t * d - s) * (2 * PI) / p ) + c + b );
}

real32 Elastic::ease_in_out(real32 t, real32 b, real32 c, real32 d)
{
  if(t == 0) {
    return b;
  }
  if( (t /= d / 2) == 2) {
    return b + c;
  }

  float p = d * (.3f * 1.5f);
  float a = c;
  float s = p / 4;

  if(t < 1) {
    float postFix = a * pow(2, 10 * (t -= 1)); // postIncrement is evil
    return -.5f * (postFix * sin( (t * d - s) * (2 * PI) / p )) + b;
  }

  float postFix =  a * pow(2, -10 * (t -= 1)); // postIncrement is evil

  return postFix * sin( (t * d - s) * (2 * PI) / p ) * .5f + c + b;
}

// ...Expo...

real32 Expo::ease_in(real32 t, real32 b, real32 c, real32 d)
{
  return(t == 0) ? b : c * pow(2, 10 * (t/d - 1) ) + b;
}

real32 Expo::ease_out(real32 t, real32 b, real32 c, real32 d)
{
  return(t == d) ? b+c : c * (-pow(2, -10 * t/d) + 1) + b;
}

real32 Expo::ease_in_out(real32 t, real32 b, real32 c, real32 d)
{
  if(t == 0) {
    return b;
  }

  if(t == d) {
    return b+c;
  }

  if( (t/=d/2) < 1) {
    return c/2 * pow(2, 10 * (t - 1) ) + b;
  }

  return c/2 * ( -pow(2, -10 * --t) + 2) + b;
}

// ...Sine...

real32 Sine::ease_in(real32 t, real32 b, real32 c, real32 d)
{
  return -c * cos(t / d * (PI / 2) ) + c + b;
}

real32 Sine::ease_out(real32 t, real32 b, real32 c, real32 d)
{
  return c * sin(t / d * (PI / 2) ) + b;
}

real32 Sine::ease_in_out(real32 t, real32 b, real32 c, real32 d)
{
  return -c / 2 * ( cos(PI* t / d) - 1) + b;
}

#pragma clang diagnostic pop

} // namespace nom

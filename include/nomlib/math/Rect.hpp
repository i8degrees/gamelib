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
#ifndef NOMLIB_MATH_RECT_HEADERS
#define NOMLIB_MATH_RECT_HEADERS

#include "nomlib/config.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/ptree/Value.hpp"

namespace nom {

/// \brief Delimiter character to use with << operator
const std::string RECT_DELIMITER = ", ";

/// \brief Rectangle shape container
///
/// \remarks This class originated from the sf::Rect class of the SFML library.
template<typename T>
struct Rect
{
  /// Default constructor; initialize values to Rect<T>::null
  Rect ( void ) :
    x ( -1 ),
    y ( -1 ),
    w ( -1 ),
    h ( -1 )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// Destructor.
  ~Rect ( void )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// Construct a Rectangle from coordinates
  Rect ( T left, T top, T w, T h ) :
    x ( left ),
    y ( top ),
    w ( w ),
    h ( h )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// \brief Construct an object from a Point2 and Size2 container types.
  Rect ( const Point2<T>& pos, const Size2<T>& size )  :
    x ( pos.x ),
    y ( pos.y ),
    w ( size.w ),
    h ( size.h )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// \brief Copy constructor
  ///
  /// \remarks The explicit keyword here will result in compile-time errors
  /// in any instance that it finds incompatible casting occurring, such as if
  /// you try to down-cast a Rect<int> to a Rect<float>.
  template <typename U>
  explicit Rect ( const Rect<U>& copy ) :
    x { static_cast<T> ( copy.x ) },
    y { static_cast<T> ( copy.y ) },
    w { static_cast<T> ( copy.w ) },
    h { static_cast<T> ( copy.h ) }
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// \brief Obtain a reference of the object.
  inline const Rect<T>& get ( void ) const
  {
    return *this;
  }

  /// Check to see if input coordinates X and Y are within the bounds
  /// of this object (think: colliding)
  bool contains ( T x, T y ) const
  {
    return  (
              ( x >= this->x && x <= this->x + this->w  ) &&
              ( y >= this->y && y <= this->y + this->h )
            );
  }

  /// Check to see if an input Point2 container are within the bounds
  /// of this object (think: colliding)
  bool contains ( const Point2<T>& pos ) const
  {
    return this->contains ( pos.x, pos.y );
  }

  /// Checks to see if our rectangle overlaps with another
  bool intersects ( const Rect<T>& rectangle ) const
  {
    T leftA, leftB = 0;
    T rightA, rightB = 0;
    T topA, topB = 0;
    T bottomA, bottomB = 0;

    // Calculate sides of RectA
    leftA = this->x;
    rightA = leftA + this->w;
    topA = this->y;
    bottomA = topA + this->h;

    // Calculate sides of RectB
    leftB = rectangle.x;
    rightB = leftB + rectangle.w;
    topB = rectangle.y;
    bottomB = topB + rectangle.h;

    if ( bottomA <= topB ) return false; // No collision
    if ( topA >= bottomB ) return false; // No collision
    if ( rightA <= leftB ) return false; // No collision
    if ( leftA >= rightB ) return false; // No collision

    return true; // Collision!
  }

  /// \brief Save an object to a nom::Value object.
  ///
  /// \todo Return an Array instead of object?
  /// \todo Rename member key 'width' to 'w'
  /// \todo Rename member key 'h' to 'h'
  const Object serialize( void ) const
  {
    Object object;

    object["x"] = this->x;
    object["y"] = this->y;
    object["width"] = this->w;
    object["height"] = this->h;

    return object;
  }

  /// \returns The positioning coordinates of the rectangle.
  const Point2i position( void ) const
  {
    return Point2i( NOM_SCAST( T, this->x ), NOM_SCAST( T, this->y ) );
  }

  /// \returns The width and height dimensions of the rectangle.
  const Size2i size( void ) const
  {
    return Size2i( NOM_SCAST( T, this->w ), NOM_SCAST( T, this->h ) );
  }

  /// \brief Null value
  ///
  /// \remarks  Null value implementation depends on signed (negative) numbers.
  static const Rect null;

  /// Left coordinate of the rectangle (X coordinate of Rectangle)
  T x;
  /// Top coordinate of the rectangle (Y coordinate of Rectangle)
  T y;
  /// Width of the rectangle (right side of Rectangle)
  T w;
  /// Height of the rectangle (bottom side of Rectangle)
  T h;
};

/// Pretty print the Rectangle using the following format string:
///
/// <Rect.x>, <Rect.y>, <Rect.w>, <Rect.h>
///
/// \note This is a convenience for us, so that we are able to quickly debug
/// an object:
///
/// \code
///
/// NOM_DUMP( my_rect );
///
/// # ...or...
///
/// std::cout << my_rect << std::endl;
/// \endcode
template <typename T>
inline std::ostream& operator <<( std::ostream& os, const Rect<T>& rect )
{
  os
  << rect.x
  << RECT_DELIMITER
  << rect.y
  << RECT_DELIMITER
  << rect.w
  << RECT_DELIMITER
  << rect.h;
  return os;
}

/// \brief Method overload of binary operator == (equality)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Compare the equality between two objects.
///
/// \returns True if left operand is equal to the right operand.
template <typename T>
inline bool operator == ( const Rect<T>& lhs, const Rect<T>& rhs )
{
  return  ( lhs.x == rhs.x )  &&  ( lhs.w == rhs.w )
                              &&
          ( lhs.y == rhs.y )  &&  ( lhs.h == rhs.h );
}

/// \brief Method overload of binary operator != (not equal)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Compare the inequality between two objects.
///
/// \returns True if left operand is not equal to the right operand.
template <typename T>
inline bool operator != ( const Rect<T>& lhs, const Rect<T>& rhs )
{
  return ! ( lhs == rhs );
}

/// \brief Method overload of binary operator + (Addition)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \returns Addition of both objects.
template <typename T>
inline Rect<T> operator + ( const Rect<T>& lhs, const Rect<T>& rhs )
{
  return Rect<T>  ( lhs.x + rhs.x,
                    lhs.y + rhs.y,
                    lhs.w + rhs.w,
                    lhs.h + rhs.h
                  );
}

/// \brief Method overload of binary operator ++ (Addition by 1)
///
/// \param rhs Right operand.
///
/// \returns Addition of the right operand.
template <typename T>
inline Rect<T> operator ++ ( const Rect<T>& rhs )
{
  return Rect<T>  ( ++rhs.x,
                    ++rhs.y,
                    ++rhs.w,
                    ++rhs.h
                  );
}

/// \brief Method overload of binary operator - (subtraction)
///
/// \param rhs Right operand.
///
/// \returns Opposite of the object.
template <typename T>
inline Rect<T> operator -( const Rect<T>& rhs )
{
  return Rect<T>  ( -rhs.x,
                    -rhs.y,
                    -rhs.w
                    -rhs.h
                  );
}

/// \brief Method overload of binary operator - (subtraction)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \returns Subtraction of both objects.
template <typename T>
inline Rect<T> operator -( const Rect<T>& lhs, const Rect<T>& rhs )
{
  return Rect<T>  ( lhs.x - rhs.x,
                    lhs.y - rhs.y,
                    lhs.w - rhs.w,
                    lhs.h - rhs.h
                  );
}

/// \brief Method overload of binary operator -- (subtraction by 1)
///
/// \param rhs Right operand.
///
/// \returns Subtraction of the right operand.
template <typename T>
inline Rect<T> operator -- ( const Rect<T>& rhs )
{
  return Rect<T>  ( --rhs.x,
                    --rhs.y,
                    --rhs.w,
                    --rhs.h
                  );
}

/// \brief Method overload of binary operator * (Multiplication)
///
/// \param rhs Left operand.
/// \param rhs Right operand.
///
/// \returns Multiplication of the right operand.
template <typename T>
inline Rect<T> operator * ( const Rect<T>& lhs, const Rect<T>& rhs )
{
  return Rect<T>  ( lhs.x * rhs.x,
                    lhs.y * rhs.y,
                    lhs.w * rhs.w,
                    lhs.h * rhs.h
                  );
}

/// \brief Method overload of binary operator / (Division)
///
/// \param rhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Division of both objects; result is assigned to the left operand.
///
/// \returns Reference to the left operand.
template <typename T>
inline Rect<T> operator /( const Rect<T>& lhs, const Rect<T>& rhs )
{
  return Rect<T>  ( lhs.x / rhs.x,
                    lhs.y / rhs.y,
                    lhs.w / rhs.w,
                    lhs.h / rhs.h
                  );
}

/// \brief Method overload of binary operator += (Addition)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Addition of both objects; result is assigned to the left
/// operand.
///
/// \returns Reference to left operand,
template <typename T>
inline Rect<T>& operator +=( Rect<T>& lhs, const Rect<T>& rhs )
{
  lhs.x += rhs.x;
  lhs.y += rhs.y;
  lhs.w += rhs.w;
  lhs.h += rhs.h;

  return lhs;
}

/// \brief Method overload of binary operator -= (Subtraction)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Subtraction of both objects; result is assigned to the left
/// operand.
///
/// \returns Reference to left operand.
template <typename T>
inline Rect<T>& operator -=( Rect<T>& lhs, const Rect<T>& rhs )
{
  lhs.x -= rhs.x;
  lhs.y -= rhs.y;
  lhs.w -= rhs.w;
  lhs.h -= rhs.h;

  return lhs;
}

/// \brief Method overload of binary operator *= (Multiplication)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Multiplication of both objects; result is assigned to the
/// left operand.
///
/// \returns Reference to left operand.
///
/// \todo See tests/math.cpp at Point2 Unit Test 2, Result[1]
template <typename T>
inline Rect<T>& operator *=( Rect<T>& lhs, const Rect<T>& rhs )
{
  lhs.x *= rhs.x;
  lhs.y *= rhs.y;
  lhs.w *= rhs.w;
  lhs.h *= rhs.h;

  return lhs;
}

/// \brief Method overload of binary operator /= (Division)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Division of both objects; result is assigned to the
/// left operand.
///
/// \returns Reference to left operand.
template <typename T>
inline Rect<T>& operator /=( Rect<T>& lhs, const Rect<T>& rhs )
{
  lhs.x /= rhs.x;
  lhs.y /= rhs.y;
  lhs.w /= rhs.w;
  lhs.h /= rhs.h;

  return lhs;
}

/// Rect object defined using signed integers
typedef Rect<int> IntRect;

/// Rect object defined using floating point numbers
typedef Rect<float> FloatRect;

/// Rect object defined using double precision floating point numbers
typedef Rect<double> DoubleRect;

} // namespace nom

#endif // include guard defined

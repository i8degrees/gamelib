/******************************************************************************

    Simple UI interface for drawing a styled message box

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_MESSAGEBOX_HEADERS
#define NOMLIB_SDL_MESSAGEBOX_HEADERS

#include <iostream>
#include <string>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/sys/Logger.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/gfx/IDrawable.hpp"
#include "nomlib/gfx/Canvas.hpp"
#include "nomlib/gfx/Gradient.hpp"
#include "nomlib/gfx/Line.hpp"

namespace nom {

class MessageBox: public IDrawable,     // "is a" relationship
                  public Transformable  // "has a" relationship
{
  public:
    MessageBox ( void );
    MessageBox  ( int32 x, int32 y, int32 width, int32 height,
                      const std::vector<Color> border_colors,
                      const Gradient& background = Gradient()
                    );
    virtual ~MessageBox ( void );

    bool isEnabled ( void );
    void disable ( void );
    void enable ( void );

    void Update ( void );
    void Draw ( void* video_buffer ) const;

  private:
    void initialize ( void );
    typedef std::vector<std::shared_ptr<IDrawable>> drawable_t;
    drawable_t lines;
    Gradient background;
    Canvas box;

    std::vector<Color> window_borders;
    // top border, left border, bottom border, right border; each defined border
    // has two rows of pixels that can be set to a custom color, so ...
    // window_border[0] && window_border[1] contains the top borders,
    // window_border[2] && window_border[3] contains the left borders, and so on.
    //
    // Each border color has three values: red, green & blue.
    //
    // window_border[0].getRed(), window_border[0].getGreen(), window_border[0].getBlue()
    //
    bool enabled; // shown or not
};


} // namespace nom

#endif // NOMLIB_SDL_MESSAGEBOX_HEADERS defined
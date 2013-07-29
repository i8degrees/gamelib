/******************************************************************************

    Abstraction of SDL Surfaces

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "nomlib/gfx/Canvas.hpp"

namespace nom {
  namespace priv {

void Canvas_FreeSurface ( SDL_Surface* video_buffer )
{
  SDL_FreeSurface ( video_buffer );
}

  } // namespace priv
} // namespace nom

namespace nom {

Canvas::Canvas ( void )  : canvas_buffer ( nullptr, nom::priv::Canvas_FreeSurface ),
                                        coords ( 0, 0, -1, -1 ), // only x, y position is used in blitting
                                        offsets ( 0, 0, -1, -1 ) // only the width, height is used in source blitting
{
NOM_LOG_CLASSINFO;
}

Canvas::Canvas ( void* video_buffer )  : canvas_buffer ( static_cast<SDL_Surface*> ( video_buffer ), nom::priv::Canvas_FreeSurface )
{
NOM_LOG_CLASSINFO;

  this->offsets.setSize ( this->getCanvasWidth(), this->getCanvasHeight() );
}

Canvas::Canvas ( const Canvas& other ) : canvas_buffer ( static_cast<SDL_Surface*> ( other.canvas_buffer.get() ), nom::priv::Canvas_FreeSurface ),
                                                          coords ( other.coords.x, other.coords.y ), offsets ( other.offsets.width, other.offsets.height )
{
NOM_LOG_CLASSINFO;
}

Canvas::Canvas ( uint32_t flags, int32_t width, int32_t height, int32_t bitsPerPixel, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask )
{
NOM_LOG_CLASSINFO;

  this->canvas_buffer.reset(); // nullptr

  this->canvas_buffer = std::shared_ptr<void> ( SDL_CreateRGBSurface ( flags, width, height, bitsPerPixel, Rmask, Gmask, Bmask, Amask ), nom::priv::Canvas_FreeSurface );
}

Canvas::Canvas ( void* pixels, int32_t width, int32_t height, int32_t depth, int32_t pitch, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask )
{
NOM_LOG_CLASSINFO;

  this->canvas_buffer.reset(); // nullptr

  this->canvas_buffer = std::shared_ptr<void> ( SDL_CreateRGBSurfaceFrom ( pixels, width, height, depth, pitch, Rmask, Gmask, Bmask, Amask ), nom::priv::Canvas_FreeSurface );
}

Canvas::~Canvas ( void )
{
NOM_LOG_CLASSINFO;

  this->canvas_buffer.reset(); // nullptr
}

bool Canvas::valid ( void ) const
{
  if ( this->canvas_buffer.get() != nullptr )
    return true;
  else
    return false;
}

void Canvas::setCanvas ( void* video_buffer )
{
  this->canvas_buffer.reset ( static_cast<SDL_Surface*> ( video_buffer ), nom::priv::Canvas_FreeSurface );
}

void Canvas::setPosition ( const Coords& coords_ )
{
  this->coords = coords_;
}

void Canvas::setOffsets ( const Coords& offsets_ )
{
  this->offsets = offsets_;
}

const int32_t Canvas::getCanvasWidth ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->w;
}

const int32_t Canvas::getCanvasHeight ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->h;
}

uint32_t Canvas::getCanvasFlags ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->flags;
}

uint16_t Canvas::getCanvasPitch ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->pitch;
}

void* Canvas::getCanvasPixels ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->pixels;
}

void* Canvas::getCanvasPixelsFormat ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->format;
}

const Coords Canvas::getCanvasBounds ( void ) const
{
  SDL_Rect clip_buffer; // temporary storage struct
  Coords clip_bounds; // transferred values from SDL_Rect clip_buffer

  // Return values are put into the clip_buffer SDL_Rect after executing:
  SDL_GetClipRect ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ) , &clip_buffer );

  // Now transfer the values into our preferred data container type
  clip_bounds = Coords ( clip_buffer.x, clip_buffer.y, clip_buffer.w, clip_buffer.h );

  return clip_bounds;
}

void Canvas::setCanvasBounds ( const Coords& clip_bounds )
{
  SDL_Rect clip = getSDL_Rect ( clip_bounds ); // temporary storage struct for setting

  // As per libSDL docs, if SDL_Rect is nullptr, the clipping rectangle is set
  // to the full size of the surface
  SDL_SetClipRect ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), &clip );
}

nom::int32 Canvas::getCanvasColorDepth ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );

  switch ( buffer->format->BytesPerPixel )
  {
    default: return -1; break; // Unsupported color depth

    case 1: return 8; break; // 8-bit
    case 2: return 16; break; // 16-bit
    case 3: return 24; break; // 24-bit
    case 4: return 32; break; // 32-bit
  }
}

bool Canvas::getCanvasLock ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->locked;
}

bool Canvas::loadFromImage ( const std::string& filename, const Color& colorkey, bool use_cache, uint32 flags )
{
  Image image;

  // By default -- for peace of mind above all else -- we have caching turned
  // off
  if ( use_cache )
  {
    priv::ObjectCache cache;

    this->canvas_buffer = cache.getObject ( filename );

    if ( this->canvas_buffer == nullptr )
    {
      this->canvas_buffer = cache.addObject ( filename, image.loadFromFile ( filename ) );
    }
  }
  else // Do not use the object cache
  {
    this->canvas_buffer = std::shared_ptr<void> ( image.loadFromFile ( filename ) );
  }

  // Validate our obtained data is good before further processing
  if ( this->valid() == false )
  {
NOM_LOG_ERR ( "Could not load canvas image file: " + filename );
    return false;
  }

  // We produce a segmentation fault here if we do not have SDL's video
  // subsystem initialized here before making the following calls, so we
  // skip the calls below if it is not and warn the end-user.
  if ( SDL_WasInit ( SDL_INIT_VIDEO ) != 0 )
  {
    if ( flags & SDL_SRCCOLORKEY )
      this->setTransparent ( colorkey, flags );

    if ( flags & SDL_SRCALPHA )
      this->displayFormatAlpha(); // Optimized video surface with an alpha channel
    else
      this->displayFormat(); // Optimized video surface without an alpha channel
  }
  else
  {
NOM_LOG_INFO ( "The video subsystem has not yet been initialized: surface transparency and format conversion has been skipped." );
  }

  // Update our canvas clipping bounds with the new source
  this->offsets.setSize ( this->getCanvasWidth(), this->getCanvasHeight() );

  return true;
}

void Canvas::Draw ( void* video_buffer ) const
{
  // temporary vars to store our wrapped Coords
  SDL_Rect blit_coords = getSDL_Rect ( this->coords );
  SDL_Rect blit_offsets = getSDL_Rect ( this->offsets );

  // Perhaps also check to see if video_buffer is nullptr?
  if ( this->valid() )
  {
    if ( blit_offsets.w != -1 && blit_offsets.h != -1 )
    {
      if ( SDL_BlitSurface ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), &blit_offsets, static_cast<SDL_Surface*> ( video_buffer ), &blit_coords ) != 0 )
NOM_LOG_ERR ( SDL_GetError() );
        return;
    }
    else
    {
      if ( SDL_BlitSurface ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), nullptr, (SDL_Surface*) video_buffer, &blit_coords ) != 0 )
NOM_LOG_ERR ( SDL_GetError() );
        return;
    }
  }
}

bool Canvas::Update ( void* video_buffer )
{
  if ( SDL_Flip ( (SDL_Surface*) video_buffer ) != 0 )
  {
NOM_LOG_ERR ( SDL_GetError() );
    return false;
  }
  return true;
}

bool Canvas::setAlpha ( uint8_t opacity, uint32_t flags )
{
NOM_ASSERT ( ! ( opacity > SDL_ALPHA_OPAQUE ) || ( opacity < SDL_ALPHA_TRANSPARENT ) );

  if ( SDL_SetAlpha ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), flags, static_cast<uint32_t>( opacity ) ) == -1 )
  {
NOM_LOG_ERR ( SDL_GetError() );
    return false;
  }

  return true;
}

bool Canvas::setTransparent ( const Color& color, uint32_t flags )
{
  uint32_t transparent_color = 0;

  // TODO: Alpha value needs testing
  transparent_color = getColorAsInt ( this->getCanvasPixelsFormat(), color );

  if ( SDL_SetColorKey ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), flags, transparent_color ) != 0 )
  {
NOM_LOG_ERR ( SDL_GetError() );
    return false;
  }

  return true;
}

bool Canvas::displayFormat ( void )
{
  this->canvas_buffer.reset ( SDL_DisplayFormat ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ) ), nom::priv::Canvas_FreeSurface );

NOM_ASSERT ( this->valid() );

  return true;
}

bool Canvas::displayFormatAlpha ( void )
{
  this->canvas_buffer.reset ( SDL_DisplayFormatAlpha ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ) ), nom::priv::Canvas_FreeSurface );

NOM_ASSERT ( this->valid() );

  return true;
}

void Canvas::clear ( const Color& color ) const
{
  Rectangle rect ( Coords ( 0, 0, this->getCanvasWidth(), this->getCanvasHeight() ), color );

  rect.Draw ( this->canvas_buffer.get() );
}

bool Canvas::mustLock ( void* video_buffer ) const
{
  if ( SDL_MUSTLOCK ( static_cast<SDL_Surface*> ( video_buffer ) ) )
    return true;
  else
    return false;
}

bool Canvas::Lock ( void* video_buffer ) const
{
  if ( this->mustLock ( video_buffer ) )
    SDL_LockSurface ( static_cast<SDL_Surface*> ( video_buffer ) );
  else
    return false;
  return true;
}

void Canvas::Unlock ( void* video_buffer ) const
{
  SDL_UnlockSurface ( static_cast<SDL_Surface*> ( video_buffer ) );
}

int32 Canvas::getPixel ( int32 x, int32 y )
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );

  switch ( this->getCanvasColorDepth() )
  {
    default: return -1; break; // Unsupported

    case 8:
    {
      uint8* pixels = static_cast<uint8*> ( buffer->pixels );

      return pixels[ ( y * buffer->pitch ) + x ];
    }
    break;

    case 16:
    {
      uint16* pixels = static_cast<uint16*> ( buffer->pixels );

      return pixels[ ( y * buffer->pitch/2 ) + x ];
    }
    break;

    case 24:
    {
      uint8* pixels = static_cast<uint8*> ( buffer->pixels );

      return pixels[ ( y * buffer->pitch ) + x ];
    }
    break;

    case 32:
    {
      uint32* pixels = static_cast<uint32*> ( buffer->pixels );

      return pixels[ ( y * buffer->pitch/4 ) + x ];
    }
    break;
  } // end switch
}

void Canvas::scale2x ( SDL_Surface* source_buffer, SDL_Surface* destination_buffer )
{
  nom::int32 looph, loopw;

  nom::uint8* srcpix = static_cast<nom::uint8*> ( source_buffer->pixels );
  nom::uint8* dstpix = static_cast<nom::uint8*> ( destination_buffer->pixels );

  const nom::int32 srcpitch = source_buffer->pitch;
  const nom::int32 dstpitch = destination_buffer->pitch;
  const nom::int32 width = source_buffer->w;
  const nom::int32 height = source_buffer->h;

  switch ( this->getCanvasColorDepth() )
  {
    default:
    {
NOM_LOG_ERR ( "Could not determine color depth -- aborting call." );
      return;
    }
    break; // Unsupported color depth?

    case 8:
    {
      nom::uint8 E0, E1, E2, E3, B, D, E, F, H;
      for(looph = 0; looph < height; ++looph)
      {
        for(loopw = 0; loopw < width; ++ loopw)
        {
          B = *(nom::uint8*)(srcpix + (std::max ( 0, looph - 1 ) * srcpitch ) + ( 1 * loopw ) );
          D = *(nom::uint8*)(srcpix + ( looph * srcpitch ) + ( 1 * std::max ( 0,loopw - 1 ) ) );
          E = *(nom::uint8*)(srcpix + ( looph * srcpitch ) + ( 1 * loopw ) );
          F = *(nom::uint8*)(srcpix + ( looph * srcpitch ) + ( 1 * std::min ( width - 1,loopw + 1 ) ) );
          H = *(nom::uint8*)(srcpix + ( std::min ( height - 1, looph + 1 ) * srcpitch ) + ( 1 * loopw ) );

          E0 = D == B && B != F && D != H ? D : E;
          E1 = B == F && B != D && F != H ? F : E;
          E2 = D == H && D != B && H != F ? D : E;
          E3 = H == F && D != H && B != F ? F : E;

          *(nom::uint8*)(dstpix + looph*2*dstpitch + loopw*2*1) = E0;
          *(nom::uint8*)(dstpix + looph*2*dstpitch + (loopw*2+1)*1) = E1;
          *(nom::uint8*)(dstpix + (looph*2+1)*dstpitch + loopw*2*1) = E2;
          *(nom::uint8*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*1) = E3;
        }
      }
    break;
    }
    case 16:
    {
      nom::uint16 E0, E1, E2, E3, B, D, E, F, H;
      for(looph = 0; looph < height; ++looph)
      {
        for(loopw = 0; loopw < width; ++ loopw)
        {
          B = *(nom::uint16*)(srcpix + ( std::max ( 0, looph - 1 ) * srcpitch ) + ( 2 * loopw ) );
          D = *(nom::uint16*)(srcpix + ( looph * srcpitch ) + ( 2 * std::max ( 0, loopw - 1 ) ) );
          E = *(nom::uint16*)(srcpix + ( looph * srcpitch ) + ( 2 * loopw ) );
          F = *(nom::uint16*)(srcpix + ( looph * srcpitch ) + ( 2 * std::min ( width - 1, loopw + 1 ) ) );
          H = *(nom::uint16*)(srcpix + ( std::min ( height - 1, looph + 1 ) * srcpitch ) + ( 2 * loopw ) );

          E0 = D == B && B != F && D != H ? D : E;
          E1 = B == F && B != D && F != H ? F : E;
          E2 = D == H && D != B && H != F ? D : E;
          E3 = H == F && D != H && B != F ? F : E;

          *(nom::uint16*)(dstpix + looph*2*dstpitch + loopw*2*2) = E0;
          *(nom::uint16*)(dstpix + looph*2*dstpitch + (loopw*2+1)*2) = E1;
          *(nom::uint16*)(dstpix + (looph*2+1)*dstpitch + loopw*2*2) = E2;
          *(nom::uint16*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*2) = E3;
        }
      }
    break;
    }
    case 24:
    {
      nom::int32 E0, E1, E2, E3, B, D, E, F, H;
      for(looph = 0; looph < height; ++looph)
      {
        for(loopw = 0; loopw < width; ++ loopw)
        {
          B = SCALE2x_READINT24(srcpix + (std::max(0,looph-1)*srcpitch) + (3*loopw));
          D = SCALE2x_READINT24(srcpix + (looph*srcpitch) + (3*std::max(0,loopw-1)));
          E = SCALE2x_READINT24(srcpix + (looph*srcpitch) + (3*loopw));
          F = SCALE2x_READINT24(srcpix + (looph*srcpitch) + (3*std::min(width-1,loopw+1)));
          H = SCALE2x_READINT24(srcpix + (std::min(height-1,looph+1)*srcpitch) + (3*loopw));

          E0 = D == B && B != F && D != H ? D : E;
          E1 = B == F && B != D && F != H ? F : E;
          E2 = D == H && D != B && H != F ? D : E;
          E3 = H == F && D != H && B != F ? F : E;

          SCALE2x_WRITEINT24((dstpix + looph*2*dstpitch + loopw*2*3), E0);
          SCALE2x_WRITEINT24((dstpix + looph*2*dstpitch + (loopw*2+1)*3), E1);
          SCALE2x_WRITEINT24((dstpix + (looph*2+1)*dstpitch + loopw*2*3), E2);
          SCALE2x_WRITEINT24((dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*3), E3);
        }
      }
    break;
    }

    case 32:
    {
      nom::uint32 E0, E1, E2, E3, B, D, E, F, H;
      for(looph = 0; looph < height; ++looph)
      {
        for(loopw = 0; loopw < width; ++ loopw)
        {
          B = *(nom::uint32*)(srcpix + (std::max(0,looph-1)*srcpitch) + (4*loopw));
          D = *(nom::uint32*)(srcpix + (looph*srcpitch) + (4*std::max(0,loopw-1)));
          E = *(nom::uint32*)(srcpix + (looph*srcpitch) + (4*loopw));
          F = *(nom::uint32*)(srcpix + (looph*srcpitch) + (4*std::min(width-1,loopw+1)));
          H = *(nom::uint32*)(srcpix + (std::min(height-1,looph+1)*srcpitch) + (4*loopw));

          E0 = D == B && B != F && D != H ? D : E;
          E1 = B == F && B != D && F != H ? F : E;
          E2 = D == H && D != B && H != F ? D : E;
          E3 = H == F && D != H && B != F ? F : E;

          *(nom::uint32*)(dstpix + looph*2*dstpitch + loopw*2*4) = E0;
          *(nom::uint32*)(dstpix + looph*2*dstpitch + (loopw*2+1)*4) = E1;
          *(nom::uint32*)(dstpix + (looph*2+1)*dstpitch + loopw*2*4) = E2;
          *(nom::uint32*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*4) = E3;
        }
      }
    break;
    }
  } // switch (BytesPerPixel)
}

Canvas& Canvas::operator = ( const Canvas& other )
{
  this->canvas_buffer = other.canvas_buffer;

  return *this;
}


} // namespace nom

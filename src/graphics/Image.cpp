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
#include "nomlib/graphics/Image.hpp"

#include "nomlib/graphics/scale2x/scale2x.hpp"
#include "nomlib/graphics/hqx/hqx.hpp"

namespace nom {

Image::Image ( void ) :
  image_ ( nullptr, priv::FreeSurface )
{
  NOM_LOG_TRACE ( NOM );
}

Image::~Image ( void )
{
  NOM_LOG_TRACE ( NOM );
}

Image::Image ( const Image& copy )  :
  image_ { copy.image() },
  position_ { copy.position() },
  bounds_ { copy.bounds_ }
{
  // ...
}

Image& Image::operator = ( const Image& other )
{
  this->image_ = other.image_;
  this->position_ = other.position();
  this->bounds_ = other.bounds_;

  return *this;
}

SDL_SURFACE::RawPtr Image::clone ( void ) const
{
  // Find the optimal pixel format
  RendererInfo caps = Window::caps( Window::context() );

  return SDL_ConvertSurfaceFormat ( this->image(), caps.optimal_texture_format(), 0 );
}

SDL_SURFACE::RawPtr Image::image ( void ) const
{
  return this->image_.get();
}

SDL_Texture* Image::texture ( void ) const
{
  return SDL_CreateTextureFromSurface ( Window::context(), this->image() );
}

bool Image::valid ( void ) const
{
  if ( this->image() != nullptr ) return true;

  return false;
}

bool Image::initialize( void* pixels, int32 width, int32 height,
                        int bits_per_pixel, uint16 pitch,
                        uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask
                      )
{
  NOM_LOG_TRACE ( NOM );

  this->image_.reset ( SDL_CreateRGBSurfaceFrom ( pixels, width, height, bits_per_pixel, pitch, Rmask, Gmask, Bmask, Amask ), priv::FreeSurface );
  //this->set_bounds ( IntRect( 0, 0, width, height) );

  if ( this->valid() == false )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

bool Image::initialize  ( int width, int height, int bits_per_pixel,
                          uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask
                        )
{
  NOM_LOG_TRACE ( NOM );
  this->image_.reset ( SDL_CreateRGBSurface ( 0, width, height, bits_per_pixel, Rmask, Gmask, Bmask, Amask ), priv::FreeSurface );
  //this->set_bounds ( IntRect( 0, 0, width, height) );

  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  // If the video surface is marked for color keying transparency, we need to
  // set it up here and now!
  // if ( colorkey )
  // {
  //   if ( this->set_colorkey ( this->colorkey(), true ) == false )
  //   {
  //     NOM_LOG_ERR ( NOM, "Could not create the video surface with color key transparency." );
  //     return false;
  //   }
  // }

  return true;
}

bool Image::initialize ( SDL_SURFACE::RawPtr source )
{
  // Discover the optimal pixel format using gathered device capabilities
  RendererInfo caps = Window::caps( Window::context() );

  if ( source == nullptr )
  {
    NOM_LOG_ERR ( NOM, "Could not initialize Image from existing surface: NULL" );
    priv::FreeSurface ( source );
    return false;
  }

  this->image_.reset ( SDL_ConvertSurfaceFormat ( source, caps.optimal_texture_format(), 0 ), priv::FreeSurface );

  if ( this->valid() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not initialize Image from existing surface: " + std::string(SDL_GetError()) );
    priv::FreeSurface ( source );
    return false;
  }

  priv::FreeSurface ( source );

  return true;
}

bool Image::initialize ( const Point2i& size )
{
  RendererInfo caps = Window::caps( Window::context() );

  if ( this->create ( size, caps.optimal_texture_format() ) == false )
  {
    NOM_LOG_ERR(NOM,"Could not initialize Image using dimensions & optimal pixel format." );
    return false;
  }

  return true;
}

bool Image::create ( const Point2i& size, uint32 pixel_format )
{
  int bpp = 0; // bits per pixel
  uint32 red_mask = 0;
  uint32 green_mask = 0;
  uint32 blue_mask = 0;
  uint32 alpha_mask = 0;

  // Find the best surface format based on the requested pixel_format
  if ( SDL_BOOL( SDL_PixelFormatEnumToMasks ( pixel_format, &bpp, &red_mask, &green_mask, &blue_mask, &alpha_mask ) ) != true )
  {
    NOM_LOG_ERR( NOM, SDL_GetError() );
    return false;
  }

  if ( this->initialize ( size.x, size.y, bpp, red_mask, green_mask, blue_mask, alpha_mask ) == false )
  {
    NOM_LOG_ERR(NOM,"Could not create Image from dimensions & pixel format." );
    return false;
  }

  return true;
}

bool Image::create ( const Size2i& size, SDL_PIXELFORMAT::RawPtr pixel_format )
{
/*
  this->image_.reset ( source.image(), priv::FreeSurface );

  if ( this->valid() == false )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }
  // TODO: cache width & height?

  // Cache the size of our new Texture object with the existing surface info
  this->set_bounds ( IntRect(0, 0, source.width(), source.height()) );

  return true;
*/
/*
  if ( this->create( this->size(), pixel_format ) == false )
  {
    NOM_LOG_ERR( NOM, "Could not create Image from existing Image object." );
    return false;
  }
*/
  if ( this->initialize( size.w, size.h, pixel_format->BitsPerPixel, pixel_format->Rmask, pixel_format->Gmask, pixel_format->Bmask, pixel_format->Amask ) == false )
  {
    NOM_LOG_ERR( NOM, "Could not create Image from existing Image object." );
    return false;
  }


  return true;
}

int32 Image::width ( void ) const
{
  SDL_Surface* buffer = this->image();
  return buffer->w;
}

int32 Image::height ( void ) const
{
  SDL_Surface* buffer = this->image();
  return buffer->h;
}

void* Image::pixels ( void ) const
{
  SDL_Surface* buffer = this->image();
  return buffer->pixels;
}

uint16 Image::pitch ( void ) const
{
  SDL_Surface* buffer = this->image();
  return buffer->pitch;
}

uint8 Image::bits_per_pixel ( void ) const
{
  SDL_Surface* buffer = this->image();

  return buffer->format->BitsPerPixel;
}

const SDL_PIXELFORMAT::RawPtr Image::pixel_format ( void ) const
{
  SDL_Surface* buffer = this->image();
  return buffer->format;
}

uint32 Image::red_mask ( void ) const
{
  return this->pixel_format()->Rmask;
}

uint32 Image::green_mask ( void ) const
{
  return this->pixel_format()->Gmask;
}

uint32 Image::blue_mask ( void ) const
{
  return this->pixel_format()->Bmask;
}

uint32 Image::alpha_mask ( void ) const
{
  return this->pixel_format()->Amask;
}

const IntRect Image::bounds ( void ) const
{
  SDL_Rect bounds;

  // Return values are put into the clip_buffer SDL_Rect after executing:
  SDL_GetClipRect ( this->image(), &bounds );

  // Now transfer the values into our preferred data container type
  return IntRect(bounds.x, bounds.y, bounds.w, bounds.h);
}

bool Image::must_lock ( void ) const
{
  if ( SDL_MUSTLOCK ( this->image() ) ) return true;

  return false;
}

bool Image::locked ( void ) const
{
  return this->image()->locked;
}

uint8 Image::alpha ( void ) const
{
  uint8 alpha;

  if ( SDL_GetSurfaceAlphaMod ( this->image(), &alpha ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return Color4i::ALPHA_OPAQUE;
  }

  return alpha;
}

void Image::set_bounds ( const IntRect& bounds )
{
  SDL_Rect clip = SDL_RECT(bounds);

  // As per libSDL docs, if SDL_Rect is nullptr, the clipping rectangle is set
  // to the full size of the surface
  SDL_SetClipRect ( this->image(), &clip );
}

bool Image::load ( const std::string& filename, uint32 pixel_format )
{
  SDL_Surface *buffer = IMG_Load ( filename.c_str() );

  if ( buffer == nullptr )
  {
NOM_LOG_ERR ( NOM, IMG_GetError() );
    priv::FreeSurface ( buffer );
    return false;
  }

  this->image_.reset ( SDL_ConvertSurfaceFormat ( buffer, pixel_format, 0 ), priv::FreeSurface );
  priv::FreeSurface ( buffer );

  return true;
}

bool Image::load_bmp ( const std::string& filename, uint32 pixel_format )
{
  SDL_Surface *buffer = IMG_Load ( filename.c_str() );
  if ( buffer == nullptr )
  {
    NOM_LOG_ERR ( NOM, IMG_GetError() );
    priv::FreeSurface ( buffer );
    return false;
  }

  this->image_.reset ( SDL_ConvertSurfaceFormat ( buffer, pixel_format, 0 ), priv::FreeSurface );
  priv::FreeSurface ( buffer );

  return true;
}

bool Image::save_bmp ( const std::string& filename ) const
{
  if ( this->valid() == false )
  {
    NOM_LOG_ERR(NOM, "Image is not valid for saving output" );
    return false;
  }

  if ( SDL_SaveBMP ( this->image(), filename.c_str() ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

bool Image::save_png ( const std::string& filename ) const
{
  if ( this->valid() == false )
  {
    NOM_LOG_ERR(NOM, "Image is not valid for saving output" );
    return false;
  }

  // Undocumented method for writing out a PNG file with SDL2_image
  if ( IMG_SavePNG ( this->image(), filename.c_str() ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

const Point2i Image::size ( void ) const
{
  SDL_Surface* buffer = this->image();
  Point2i image_pos ( buffer->w, buffer->h );

  return image_pos;
}

const Color4i Image::colorkey ( void ) const
{
  SDL_Surface* buffer = this->image();
  uint32 transparent_color = 0;
  Color4i key; // Color4i::null

  if ( SDL_GetColorKey ( this->image(), &transparent_color ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return key; // Color4i::null
  }

  return nom::pixel ( transparent_color, buffer->format ); // SDL_helper function
}

const SDL_BlendMode Image::blend_mode ( void ) const
{
  SDL_BlendMode blend;

  if ( SDL_GetSurfaceBlendMode ( this->image(), &blend ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return blend;
  }

  return blend;
}

const Color4i Image::color_modulation ( void ) const
{
  SDL_Color c;

  if ( SDL_GetSurfaceColorMod ( this->image(), &c.r, &c.g, &c.b ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return Color4i::null;
  }

  return Color4i ( c.r, c.g, c.b, Color4i::ALPHA_OPAQUE );
}

const Point2i Image::position ( void ) const
{
  return this->position_;
}

bool Image::set_colorkey ( const Color4i& colorkey, bool flag )
{
  SDL_Surface* buffer = this->image();
  uint32 transparent_color = RGB ( colorkey, buffer->format );

  if ( this->valid() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not set color key: invalid image buffer." );
    priv::FreeSurface ( buffer );
    return false;
  }

  if ( SDL_SetColorKey ( buffer, SDL_BOOL(flag), transparent_color ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    priv::FreeSurface ( buffer );
    return false;
  }

  return true;
}

bool Image::RLE ( bool flag )
{
  if ( SDL_SetSurfaceRLE ( this->image(), SDL_BOOL(flag) ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }
  return true;
}

uint32 Image::pixel ( int x, int y )
{
  switch ( this->bits_per_pixel() )
  {
    case 8:
    {
      uint8* pixels = static_cast<uint8*> ( this->pixels() );

      return pixels[ ( y * this->pitch() ) + x ];
      break;
    }

    case 16:
    {
      uint16* pixels = static_cast<uint16*> ( this->pixels() );

      return pixels[ ( y * this->pitch() / 2 ) + x ];
      break;
    }

    case 24:
    {
      uint8* pixels = static_cast<uint8*> ( this->pixels() );

      return pixels[ ( y * this->pitch() ) + x ];
      break;
    }

    default: // Unknown color depth; log a debug message & assume 32-bit bpp
    {
      NOM_LOG_ERR ( NOM, "Could not determine color depth for pixel reading; assuming 32-bit" );
    }
    case 32:
    {
      uint32* pixels = static_cast<uint32*> ( this->pixels() );

      return pixels[ ( y * this->pitch()/4 ) + x ];
      break;
    }
  } // end switch
}

void Image::set_pixel ( int x, int y, const Color4i& color )
{
  uint32 c = 0;

  switch ( this->bits_per_pixel() )
  {
    case 8:
    {
      uint8* pixels = static_cast<uint8*> ( this->pixels() );
      c = RGB ( color, this->pixel_format() );

      pixels[ ( y * this->pitch() ) + x ] = c;
      break;
    }

    case 16:
    {
      uint16* pixels = static_cast<uint16*> ( this->pixels() );
      c = RGBA ( color, this->pixel_format() );

      pixels[ ( y * this->pitch()/2 ) + x ] = c;
      break;
    }

    case 24:
    {
      SDL_PixelFormat* fmt = this->image()->format;
      uint8* pixels = static_cast<uint8*> ( this->pixels() );
      c = RGBA ( color, this->pixel_format() );

      pixels[ ( y * this->pitch() ) + x ] = c;
      *(pixels + fmt->Rshift/8 ) = color.r;
      *(pixels + fmt->Gshift/8 ) = color.g;
      *(pixels + fmt->Bshift/8 ) = color.b;
      *(pixels + fmt->Ashift/8 ) = color.a;
      break;
    }

    default: // Unknown color depth; log a debug message & assume 32-bit bpp
    {
      NOM_LOG_ERR ( NOM, "Could not determine color depth for pixel writing; assuming 32-bit" );
    }
    case 32:
    {
      uint32* pixels = static_cast<uint32*> ( this->pixels() );
      c = RGBA ( color, this->pixel_format() );

      pixels[ ( y * this->pitch()/4 ) + x ] = c;
      break;
    }
  } // end switch
}

bool Image::set_blend_mode ( const SDL_BlendMode blend )
{
  if ( SDL_SetSurfaceBlendMode ( this->image(), blend ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

bool Image::lock ( void ) const
{
  if ( SDL_BOOL ( this->must_lock() ) )
  {
    if ( SDL_LockSurface ( this->image() ) != 0 )
    {
NOM_LOG_ERR ( NOM, "Could not lock video surface memory." );
      return false;
    }
  }

  return true;
}

void Image::unlock ( void ) const
{
  SDL_UnlockSurface ( this->image() );
}

bool Image::set_alpha ( uint8 opacity )
{
  NOM_ASSERT ( ! ( opacity > Color4i::ALPHA_OPAQUE ) || ( opacity < Color4i::ALPHA_TRANSPARENT ) );

  if ( SDL_SetSurfaceAlphaMod ( this->image(), opacity ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

void Image::set_position ( const Point2i& pos )
{
  this->position_.x = pos.x;
  this->position_.y = pos.y;
}

bool Image::set_color_modulation ( const Color4i& color )
{
  if ( SDL_SetSurfaceColorMod ( this->image(), color.r, color.g, color.b ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

void Image::draw ( SDL_SURFACE::RawPtr destination, const IntRect& bounds ) const
{
  SDL_Rect blit_coords = SDL_RECT ( bounds );
  SDL_Rect blit_offsets = SDL_RECT ( bounds );

  if ( blit_offsets.w != -1 && blit_offsets.h != -1 )
  {
    if ( SDL_BlitSurface ( this->image(), &blit_offsets, destination, &blit_coords ) != 0 )
    {
      NOM_LOG_ERR ( NOM, SDL_GetError() );
      return; // ERR
    }
  }
  else
  {
    if ( SDL_BlitSurface ( this->image(), nullptr, destination, &blit_coords ) != 0 )
    {
      NOM_LOG_ERR ( NOM, SDL_GetError() );
      return; // ERR
    }
  }
}

bool Image::resize ( enum Image::ResizeAlgorithm scaling_algorithm )
{
  // Current texture access state; we *MUST* have access to its pixels, so only
  // SDL_TEXTUREACCESS_STREAMING types will work for us.
  // enum Texture::Access type = this->access();

  // if ( type != Texture::Access::Streaming )
  // {
  //   NOM_LOG_ERR ( NOM, "This texture was not created with Texture::Access::Streaming." );
  //   return false;
  // }

  Size2i source_size ( this->width(), this->height() );
  Size2i destination_size;

  // Tru to ensure that our existing texture surface is OK first.
  if ( this->valid() == false )
  {
    NOM_LOG_ERR ( NOM, "The existing video surface is not valid." );
    return false;
  }

  // Temporary memory buffer (pixels surface) that we use to rescale this
  // Texture to
  Image destination;

  // Pick out the suitable scaling factor for determining the new video surface
  // width and height.

  //Size2i scale = Size2i ( this->scale_factor ( scaling_algorithm ), this->scale_factor ( scaling_algorithm ) );
  int scale = this->scale_factor ( scaling_algorithm );

  destination_size = Size2i( source_size.w * scale, source_size.h * scale );
  //destination_size = source_size * scale;

  // Create a memory buffer twice our size, using our existing pixel format
  destination.create( destination_size, this->pixel_format() );

  if ( destination.valid() == false )
  {
    NOM_LOG_ERR ( NOM, "The destination video surface is not valid." );
    return false;
  }

  // Lock our texture so we can access its pixel buffer
  if ( this->lock() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not lock video surface memory for reading." );
    return false;
  }

  // Lock our destination memory for copying our texture to
  if ( destination.lock() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not lock destination video surface for writing." );
    return false;
  }

  switch ( scaling_algorithm )
  {
    default: // No rescaling algorithm has been set -- aborting...
    {
      NOM_LOG_ERR ( NOM, "No rescaling algorithm has been set." );
      this->unlock();
      destination.unlock();
      return false;
    }

    case ResizeAlgorithm::scale2x:
    {
      if ( priv::scale2x  (
                            this->pixels(),
                            destination.pixels(),
                            source_size.w,
                            source_size.h,
                            this->bits_per_pixel(),
                            this->pitch(),
                            destination.pitch()
                          ) == false )
      {
        NOM_LOG_ERR ( NOM, "Failed to resize video surface with scale2x." );
        this->unlock(); // Relinquish our read lock
        destination.unlock();
        return false;
      }
      break;
    }

    case ResizeAlgorithm::scale3x:
    {
      if ( priv::scale3x  (
                            this->pixels(),
                            destination.pixels(),
                            source_size.w,
                            source_size.h,
                            this->bits_per_pixel(),
                            this->pitch(),
                            destination.pitch()
                          ) == false )
      {
        NOM_LOG_ERR ( NOM, "Failed to resize video surface with scale3x." );
        this->unlock(); // Relinquish our read lock
        destination.unlock();
        return false;
      }
      break;
    }

    case ResizeAlgorithm::scale4x:
    {
      if ( priv::scale2x  (
                            this->pixels(),
                            destination.pixels(),
                            source_size.w,
                            source_size.h,
                            this->bits_per_pixel(),
                            this->pitch(),
                            destination.pitch()
                          ) == false )
      {
        NOM_LOG_ERR ( NOM, "Failed to resize video surface with scale4x." );
        this->unlock(); // Relinquish our read lock
        destination.unlock();
        return false;
      }
      break;
    }

    case ResizeAlgorithm::hq2x:
    {
      priv::hqxInit();
      priv::hq2x_32 (
                      static_cast<uint32_ptr> ( this->pixels() ),
                      static_cast<uint32_ptr> ( destination.pixels() ),
                      source_size.w,
                      source_size.h
                    );
      break;
    }

    case ResizeAlgorithm::hq3x:
    {
      priv::hqxInit();
      priv::hq3x_32 (
                      static_cast<uint32_ptr> ( this->pixels() ),
                      static_cast<uint32_ptr> ( destination.pixels() ),
                      source_size.w,
                      source_size.h
                    );
      break;
    }

    case ResizeAlgorithm::hq4x:
    {
      priv::hqxInit();
      priv::hq4x_32 (
                      static_cast<uint32_ptr> ( this->pixels() ),
                      static_cast<uint32_ptr> ( destination.pixels() ),
                      source_size.w,
                      source_size.h
                    );
      break;
    }
  } // end switch scaling_algorithm

  // Unlock destination memory buffer; we are done copying from the texture!
  destination.unlock();
  this->unlock();

  // Do one more sanity check on our new video surface before do the final
  // transfer
  if ( destination.valid() == false )
  {
    NOM_LOG_ERR ( NOM, "The rescaled video surface is not valid." );
    return false;
  }

  // Dump rescaled surface memory as a PNG
  #if defined (NOM_DEBUG_IMAGE_RESIZE_PNG)
    destination.save_png("rescaled_surface.png");
  #endif

  // Re-initialize our texture with the rescaled pixels from our temporary
  // software surface. There's no turning back once this is done... we will
  // need to reload the image file back into the Texture and start this all
  // over again.
  //this->create ( destination ); // STATIC access type

  //*this = Image ( destination );
  this->initialize( destination.clone() );

  return true;
}

/*Size2i*/sint Image::scale_factor ( enum Image::ResizeAlgorithm scaling_algorithm ) const
{
  switch ( scaling_algorithm )
  {
    default: return 1; // No rescaling algorithm set

    case ResizeAlgorithm::scale2x:
    case ResizeAlgorithm::hq2x:
    {
      return 2;
    }

    case ResizeAlgorithm::scale3x:
    case ResizeAlgorithm::hq3x:
    {
      return 3;
    }

    case ResizeAlgorithm::scale4x:
    case ResizeAlgorithm::hq4x:
    {
      return 4;
    }
  }
}

} // namespace nom

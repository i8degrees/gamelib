/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#include "nomlib/graphics/TrueTypeFont.hpp"

namespace nom {

TrueTypeFont::TrueTypeFont ( void ) :
  font_size_ ( 12 ),
  newline_ ( 0 ),
  spacing_ ( 0 ),
  use_cache_ ( false ),
  type_ ( IFont::FontType::TrueTypeFont )
{
  NOM_LOG_TRACE ( NOM );

  if ( TTF_Init () == -1 )
  {
    NOM_LOG_ERR ( NOM, TTF_GetError() );
  }
}

TrueTypeFont::~TrueTypeFont ( void )
{
  NOM_LOG_TRACE ( NOM );

  // See FIXME note (nomlib/graphics/TrueTypeFont.hpp)
  this->font_.reset();

  TTF_Quit();
}

TrueTypeFont::TrueTypeFont ( const TrueTypeFont& copy )
{
  this->font_buffer_ = copy.font_buffer_;
  this->font_ = copy.font_;
  this->pages_ = copy.pages_;
  this->type_ = copy.type();
  this->font_size_ = copy.font_size();
  this->newline_ = copy.newline();
  this->spacing_ = copy.spacing();
  this->filename_ = copy.filename_;
  this->use_cache_ = copy.use_cache_;
}

IFont::SharedPtr TrueTypeFont::clone ( void ) const
{
  return TrueTypeFont::SharedPtr ( new TrueTypeFont ( *this ) );
}

bool TrueTypeFont::valid ( void ) const
{
  if ( this->font_.get() != nullptr ) return true;

  return false;
}

enum IFont::FontType TrueTypeFont::type ( void ) const
{
  return this->type_;
}

SDL_SURFACE::RawPtr TrueTypeFont::image ( void ) const
{
  return nullptr;
}

const Texture& TrueTypeFont::texture ( uint32 character_size ) /*const*/
{
  return this->pages_[character_size].texture;
}

uint TrueTypeFont::spacing ( void ) const
{
  return this->spacing_;
}

int32 TrueTypeFont::font_size ( void ) const
{
  return this->font_size_;
}

void TrueTypeFont::set_spacing ( uint spaces )
{
  this->spacing_ = spaces;
}

uint TrueTypeFont::newline ( void ) const
{
  return this->newline_;
}

void TrueTypeFont::set_newline ( uint newline )
{
  this->newline_ = newline;
}

/*
void TrueTypeFont::set_font_size ( sint point_size )
{
  sint original_font_size = this->font_size_;

  this->font_size_ = point_size;

  if ( this->rebuild() == false )
  {
NOM_LOG_ERR ( NOM, "Could not set new font size." );
    this->font_size_ = original_font_size;
    return;
  }
}
*/

/*
void TrueTypeFont::setFontStyle ( int32 style, uint8 options )
{
  switch ( style )
  {
    default: break;
    case FontStyle::Regular:
    {
      TTF_SetFontStyle ( this->font.get(), TTF_STYLE_NORMAL );
    }
    break;

    case FontStyle::Bold:
    {
      TTF_SetFontStyle ( this->font.get(), TTF_STYLE_BOLD );
    }
    break;

    case FontStyle::Italic:
    {
      TTF_SetFontStyle ( this->font.get(), TTF_STYLE_ITALIC );
    }
    break;

    case FontStyle::Underlined:
    {
      TTF_SetFontStyle ( this->font.get(), TTF_STYLE_UNDERLINE );
    }
    break;

    /// Text effect utilizing alpha channels for the appearance of gray text
    case FontStyle::Faded:
    {
      this->text_style = FontStyle::Faded;
      this->style_options = options;
    break;
    }
  }
}
*/

/*
int32 TrueTypeFont::getFontOutline ( void ) const
{
  return TTF_GetFontOutline ( this->font.get() );
}

void TrueTypeFont::setFontOutline ( int32 depth )
{
  TTF_SetFontOutline ( this->font.get(), depth );
}
*/

/*
void TrueTypeFont::setText ( const std::string& text )
{
  if ( text.length() < 1 ) return;

  this->text_buffer = text;

  // Font is not valid -- TTF_SizeText will crash if we go further
  if ( ! this->valid() ) return;

  // Update the width & height of text string, if we can
  if ( TTF_SizeText ( this->font.get(), this->text_buffer.c_str(), &this->coords.width, &this->coords.height ) == -1 )
  {
NOM_LOG_ERR ( NOM, "Failed to set font width & height." );
  }
}
*/

const Glyph& TrueTypeFont::glyph ( uint32 codepoint, uint32 character_size ) /*const*/
{
  GlyphAtlas& glyphs = this->pages_[character_size].glyphs;

  return glyphs[codepoint];
/* TODO
  GlyphAtlas::const_iterator it = glyphs.find(codepoint);

  if ( it != glyphs.end() )
  {
    return it->second;
  }
  else
  {
    // FIXME: implement support for handling this condition
    return Glyph();
  }
*/
}

bool TrueTypeFont::load ( const std::string& filename, const Color4u& colorkey,
                          bool use_cache
                        )
{
  this->font_ = std::shared_ptr<TTF_Font> ( TTF_OpenFont ( filename.c_str(), this->font_size() ), priv::TTF_FreeFont );

  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not load TTF file: " + filename );
    return false;
  }

  // Store the new filename & caching choice for future reference; primarily
  // used when rebuilding font metrics, such as when we change the font point
  // size or load a new font.
  this->filename_ = filename;
  this->use_cache_ = use_cache;

  return true;
}

/*
void TrueTypeFont::update ( void )
{
  // Font is not valid -- nothing to draw
  if ( this->valid() == false ) return;

  // No text string set -- nothing to draw
  if ( this->getText().length() < 1 ) return;

  // Update display coordinates
  this->font_buffer_.set_position ( Point2i ( this->coords.x, this->coords.y ) );

  // Update the rendered text surface here for drawing
  if ( this->rendering == RenderStyle::Shaded ) // Moderate-quality
  {
    this->font_buffer_.initialize ( TTF_RenderText_Shaded
                                  (
                                    this->font.get(),
                                    this->getText().c_str(),
                                    SDL_COLOR(this->color),
                                    // TODO; implement me -- a second color
                                    // possibility means needing two colors in
                                    // class
                                    SDL_COLOR ( Color4u ( 97, 97, 97 ) )
                                  )
                                );
  }
  else if ( this->rendering == RenderStyle::Blended ) // Highest-quality
  {
    this->font_buffer_.initialize ( TTF_RenderText_Blended
                                  (
                                    this->font.get(),
                                    this->getText().c_str(),
                                    SDL_COLOR(this->color)
                                  )
                                );
  }
  else // Low-quality rendering (the default)
  {
    this->font_buffer_.initialize ( TTF_RenderText_Solid
                                  (
                                    this->font.get(),
                                    this->getText().c_str(),
                                    SDL_COLOR(this->color)
                                  )
                                );
  }

  if ( this->text_style == FontStyle::Faded )
  {
    this->font_buffer_.set_alpha ( this->style_options );
  }
}
*/

/*
void TrueTypeFont::draw ( RenderTarget target ) const
{
  if ( this->font_buffer_.valid() )
  {
    this->font_buffer_.draw ( target );
  }
}
*/

const GlyphPage& TrueTypeFont::pages ( void ) const
{
  return this->pages_;
}

bool TrueTypeFont::build ( uint32 character_size )
{
  if ( this->load ( this->filename_, NOM_COLOR4U_BLACK, this->use_cache_ ) == false )
  {
NOM_LOG_ERR ( NOM, "Could not build font metrics." );
    return false;
  }

  return true;
}

} // namespace nom

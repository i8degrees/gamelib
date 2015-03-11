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
#include "nomlib/graphics/sprite/SpriteSheet.hpp"

// Private headers
#include "nomlib/serializers/JsonCppSerializer.hpp"
#include "nomlib/serializers/JsonCppDeserializer.hpp"

namespace nom {

// Static initializations
const VersionInfo SpriteSheet::VERSION = VersionInfo(0,4,1);

SpriteSheet::SpriteSheet() :
  sheet_filename_("\0"),
  sheet_spacing_(0),
  sheet_padding_(0),
  sheet_width_(0),
  sheet_height_(0),
  total_frames_(0)
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_RENDER, nom::NOM_LOG_PRIORITY_VERBOSE );
}

SpriteSheet::~SpriteSheet()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_RENDER, nom::NOM_LOG_PRIORITY_VERBOSE );
}

const IntRect& SpriteSheet::dimensions(int index) const
{
  NOM_ASSERT( index < this->sheet_.size() );

  return this->sheet_.at(index);
}

int SpriteSheet::frames() const
{
  return this->sheet_.size();
}

bool SpriteSheet::empty() const
{
  if( this->frames() < 1 ) return true;

  return false;
}

std::string SpriteSheet::version() const
{
  return this->VERSION.version_string();
}

const std::string& SpriteSheet::sheet_filename() const
{
  return this->sheet_filename_;
}

int SpriteSheet::sheet_width() const
{
  return this->sheet_width_;
}

int SpriteSheet::sheet_height() const
{
  return this->sheet_height_;
}

int SpriteSheet::sheet_padding() const
{
  return this->sheet_padding_;
}

int SpriteSheet::sheet_spacing() const
{
  return this->sheet_spacing_;
}

int SpriteSheet::total_frames() const
{
  return this->total_frames_;
}

SpriteSheet::SharedPtr SpriteSheet::clone() const
{
  return SpriteSheet::SharedPtr ( new SpriteSheet ( *this ) );
}

bool SpriteSheet::load_file(const std::string& filename)
{
  IValueDeserializer* serializer = new JsonCppDeserializer();
  Value output; // Value buffer of resulting de-serialized input.

  if( serializer->load( filename, output ) == false )
  {
    NOM_LOG_ERR( NOM, "Unable to parse JSON file:", filename );
    return false;
  }

  return this->load_sheet_object(output);
}

bool SpriteSheet::load_sheet_object(const Value& object)
{
  // Temporary holding buffers to hold data until we are ready to commit back
  // to our class
  int pos = 0;
  std::map<int, IntRect> buffer;
  Value fp = object;
  VersionInfo file_ver;       // metadata object's 'version' container
  VersionInfo min_ver(0,4,0); // minimum required spec version for this func
  std::string fp_version;     // metadata object's 'version' string

  if( fp.null_type() ) {
    NOM_LOG_ERR( NOM, "Could not load sprite sheet: nom::Value object was null." );
    return false;
  }

  #if defined( NOM_DEBUG_SPRITE_SHEET_JSON_LOAD )
    NOM_DUMP(fp);
  #endif

  fp_version = fp["metadata"]["version"].get_string();

  if( VersionInfo::convert_version_string(fp_version, file_ver) == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not load sprite sheet: invalid 'version' string",
                  file_ver );
    return false;
  }

  if( file_ver < min_ver ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not load sprite sheet: file spec version",
                  file_ver, "is less than minimum required version", min_ver );
    return false;
  }

  // Populate our object state with parsed JSON object data
  for( auto itr = fp.begin(); itr != fp.end(); ++itr ) {
    // NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST, ".");

    if( itr->object_type() ) {

      // { "frames": {}, "metadata" }
      Object obj = itr->object();
      Value::Iterator member(itr);
      std::string key = member.key();
      // NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST, "..", key);

      if( key == "metadata" ) {
        this->sheet_filename_ = fp[key]["filename"].get_string();
        this->sheet_width_ = fp[key]["width"].get_int();
        this->sheet_height_ = fp[key]["height"].get_int();
        this->sheet_padding_ = fp[key]["padding"].get_int();
        this->sheet_spacing_ = fp[key]["spacing"].get_int();

        if( file_ver > min_ver ) {
          this->total_frames_ = fp[key]["total_frames"].get_int();
        }
      } // end if key == metadata
      else {
        for( auto itr = obj.begin(); itr != obj.end(); ++itr ) {

          // { "frames": { "0": { ... } } }
          Value::Iterator member(itr);
          std::string key = member.key();
          // NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST, "...", key);

          // ...This next line here is why we are using an ordered map; we need
          // a consistent layout order so we can do sprite frame lookups based
          // on the actual identifier (i.e.: object key), **not** the element
          // position as was previously the case with a std::vector.
          //
          // By using the element's position and not the object's frame ID, you
          // end up with a sorting problem, i.e.: position ten is not always
          // necessarily going to be equal to frame ID 10.
          //
          // Note that you cannot count on serialized JSON output to be in any
          // particular order. This is the problem that had me redesign the
          // internal storage type in the first place...
          pos = atoi( key.c_str() );

          buffer[pos].x =
            obj[key]["x"].get_int();

          buffer[pos].y =
            obj[key]["y"].get_int();

          buffer[pos].w =
            obj[key]["width"].get_int();

          buffer[pos].h =
            obj[key]["height"].get_int();

        } // end for inner loop
      } // end else
    } // end if object type
  } // end for outer loop

  if( file_ver > min_ver ) {
    NOM_ASSERT( this->sheet_.size() == this->total_frames() );
  }

  #if defined( NOM_DEBUG_SPRITE_SHEET_JSON_LOAD )
    NOM_DUMP(fp);
  #endif

  // If we have gotten this far, we assume all is well, so let's commit the data
  // as valid sprite sheet data!
  this->sheet_.clear();
  this->sheet_ = buffer;

  return true;
}

bool SpriteSheet::remove_frame(nom::size_type frame)
{
  auto res = this->sheet_.find(frame);

  if( res == this->sheet_.end() ) {
    // Not found
    return false;
  } else {

    // Found
    this->sheet_.erase(res);
    return true;
  }
}

void SpriteSheet::dump ( void ) const
{
  // Sheet vector state
  for ( auto itr = this->sheet_.begin(); itr != this->sheet_.end(); ++itr )
  {
    NOM_DUMP(itr->second.x);
    NOM_DUMP(itr->second.y);
    NOM_DUMP(itr->second.w);
    NOM_DUMP(itr->second.h);
  }

  // Sheet's meta-data
  NOM_DUMP(this->sheet_filename() );
  NOM_DUMP(this->sheet_spacing_);
  NOM_DUMP(this->sheet_padding_);
  NOM_DUMP(this->sheet_width_);
  NOM_DUMP(this->sheet_height_);
}


} // namespace nom

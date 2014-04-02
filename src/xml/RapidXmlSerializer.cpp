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
#include "nomlib/xml/RapidXmlSerializer.hpp"

namespace nom {

RapidXmlSerializer::RapidXmlSerializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

RapidXmlSerializer::~RapidXmlSerializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

bool RapidXmlSerializer::serialize( const Value& source, const std::string& output ) const
{
  std::ofstream fp;
  rapidxml::xml_document<> doc;

  this->append_decl( doc );

  if( this->write( source, doc ) == false )
  {
    NOM_LOG_ERR ( NOM, "Failed to serialize data stream." );
    return false;
  }

  fp.open( output );

  if ( ! fp.is_open() || ! fp.good() )
  {
    NOM_LOG_ERR ( NOM, "Unable to write XML output in file: " + output );
    return false;
  }

  fp << doc;

  fp.close();

  return true;
}

bool RapidXmlSerializer::unserialize( const std::string& input, Value& dest ) const
{
  std::stringstream fp_buffer;
  std::string xml_document;
  std::ifstream fp;

  fp.open( input );

  if ( ! fp.is_open() || ! fp.good() )
  {
    NOM_LOG_ERR ( NOM, "File access failure on file: " + input );
    return false;
  }

  fp_buffer << fp.rdbuf();

  xml_document = std::string( fp_buffer.str() );

  if( this->read( xml_document, dest ) == false )
  {
    NOM_LOG_ERR( NOM, "Failure to write XML document in file: " + input );
    return false;
  }

  fp.close();

  return true;
}

const std::string RapidXmlSerializer::stringify( const Value& input ) const
{
  std::stringstream os;
  // Value val;

  // if( this->write( val, doc ) == false )
  // {
  //   // TODO: Err handling
  //   NOM_STUBBED( NOM );
  //   return "\0";
  // }

  // os << doc;

  os << "\0";
  return os.str();
}

bool RapidXmlSerializer::write( const Value& source, rapidxml::xml_document<>& dest ) const
{
  uint index = 0;
  std::string root_key, key;

  // Begin iterating through the entire container; values, arrays, objects and
  // even objects within objects.
  for( Value::ConstIterator itr = source.begin(); itr != source.end(); ++itr )
  {
    ValueConstIterator member( itr );

    root_key = member.key();

    // Top-level node is an array object -- not supported.
    if( root_key == "\0" )
    {
      // TODO: Err handling
      NOM_STUBBED( NOM );
      return false;
    } // end if root_key == "\0" (Top-level node is an array)

    // Top-level node is an object.
    else if( itr->object_type() )
    {
      #if defined( NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES )
        NOM_DUMP( itr->type_name() );
        NOM_DUMP( root_key );
      #endif

      rapidxml::xml_node<>* root_node = dest.allocate_node( rapidxml::node_element, this->stralloc( root_key, dest ) );
      dest.append_node( root_node );

      nom::Object objects = itr->object();

      // Iterate through each object and store every member's key / value pair
      // we find
      for( Value::ConstIterator itr = objects.begin(); itr != objects.end(); ++itr )
      {
        nom::ValueConstIterator member( itr );

        key = member.key();

        #if defined( NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES )
          NOM_DUMP( itr->type_name() );
          NOM_DUMP( key );
        #endif

        rapidxml::xml_node<>* node = dest.allocate_node( rapidxml::node_element, this->stralloc( key, dest ) );

        if( this->serialize_object( itr->ref(), dest, node ) == false )
        {
          NOM_LOG_ERR( NOM, "Could not serialize values; invalid object???" );
          return false;
        }

        root_node->append_node( node );
      } // end for objects loop

      // EOF Object
      ++index;

    } // end if top-level object node
  } // end for source root object loop

  return true;
}

bool RapidXmlSerializer::read( /*const*/ std::string& input, Value& dest ) const
{
  rapidxml::xml_document<> doc;

  doc.parse<0>( &input[0] );

  Object obj, objects;
  for( rapidxml::xml_node<> *node = doc.first_node(); node; node = node->next_sibling() )
  {
    std::string node_key = node->name();

    #if defined( NOM_DEBUG_RAPIDXML_UNSERIALIZER_VALUES )
      NOM_DUMP( node_key );
    #endif

    if( this->unserialize_object( node, obj[node_key] ) == false )
    {
      // TODO: Err handling
      NOM_STUBBED( NOM );
      return false;
    }

    objects[node_key] = obj;
  }

  dest = obj;

  return true;
}

bool RapidXmlSerializer::write_value( const Value& object, const std::string& key, rapidxml::xml_document<>& doc, rapidxml::xml_node<>* parent ) const
{
  switch( object.type() )
  {
    default: // Unknown type
    {
      #if defined( NOM_DEBUG_JSONCPP_SERIALIZER_VALUES )
        NOM_DUMP( object.type() );
        NOM_DUMP( object );
      #endif

      // TODO: Err handling
      NOM_STUBBED( NOM );
      return false;

      break;
    }

    case Value::ValueType::Null:
    case Value::ValueType::SignedInteger:
    case Value::ValueType::UnsignedInteger:
    case Value::ValueType::RealNumber:
    case Value::ValueType::String:
    case Value::ValueType::Boolean:
    {
      #if defined( NOM_DEBUG_JSONCPP_SERIALIZER_VALUES )
        NOM_DUMP( object.type() );
      #endif

      rapidxml::xml_node<>* val = doc.allocate_node( rapidxml::node_element, this->stralloc( key, doc ), this->stralloc( object.stringify(), doc ) );
      parent->append_node( val );

      break;
    }

    case Value::ValueType::ArrayValues:
    {
      // Special case -- must be handled by a dedicated method in order to
      // support deep-depth hierarchies.
      break;
    }

    case Value::ValueType::ObjectValues:
    {
      // Special case -- must be handled by a dedicated method in order to
      // support deep-depth hierarchies.
      break;
    }
  }

  return true;
}

bool RapidXmlSerializer::serialize_array( const Value& object, const std::string& key, rapidxml::xml_document<>& doc, rapidxml::xml_node<>* parent ) const
{
  Array array;

  array = object.array();
  // NOM_DUMP( member_value );

  for( auto itr = array.begin(); itr != array.end(); ++itr )
  {
    switch( itr->type() )
    {
      default: // Unknown type
      {
        #if defined( NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES )
          NOM_DUMP( itr->type() );
          NOM_DUMP( itr->ref() );
        #endif

        // TODO: Err handling
        NOM_STUBBED( NOM );
        return false;

        break;
      }

      case Value::ValueType::Null:
      case Value::ValueType::SignedInteger:
      case Value::ValueType::UnsignedInteger:
      case Value::ValueType::RealNumber:
      case Value::ValueType::String:
      case Value::ValueType::Boolean:
      {
        std::string member_value = itr->stringify();

        #if defined( NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES )
          NOM_DUMP( key );
          NOM_DUMP( member_value );
        #endif

        // FIXME:
        //
        // rapidxml::xml_attribute<>* attrib = doc.allocate_attribute( this->stralloc( key, doc ), this->stralloc( member_value, doc ) );
        // parent->append_attribute( attrib );

        break;
      }

      case Value::ValueType::ArrayValues:
      {
        NOM_LOG_ERR( NOM, "Could not serialize values; an array cannot exist within another array." );
        return false;

        break;
      }

      case Value::ValueType::ObjectValues:
      {
        if( this->serialize_object( itr->ref(), doc, parent ) == false )
        {
          NOM_LOG_ERR( NOM, "Could not serialize values; invalid object???" );
          return false;
        }

        break;
      }
    }
  } // end for array nodes loop

  return true;
}
bool RapidXmlSerializer::serialize_object( const Value& object, rapidxml::xml_document<>& doc, rapidxml::xml_node<>* parent ) const
{
  switch( object.type() )
  {
    default: // Unknown type
    {
      #if defined( NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES )
        NOM_DUMP( object.type() );
        NOM_DUMP( object );
      #endif

      // TODO: Err handling
      NOM_STUBBED( NOM );
      return false;

      break;
    }

    // Object within object containing values (top-level).
    case Value::ValueType::Null:
    case Value::ValueType::SignedInteger:
    case Value::ValueType::UnsignedInteger:
    case Value::ValueType::RealNumber:
    case Value::ValueType::String:
    case Value::ValueType::Boolean:
    {
      // Error -- not supported (we need a member key!)

      // TODO: Err handling
      NOM_STUBBED( NOM );
      return false;

      break;
    }

    // Handle arrays within an object (top-level).
    case Value::ValueType::ArrayValues:
    {
      // Error -- not supported (we need a member key!)

      // TODO: Verify functionality
      //
      // if( this->serialize_array( object, "", doc, parent ) == false )
      // {
      //   NOM_LOG_ERR( NOM, "Could not serialize values; invalid array???" );
      //   return false;
      // }

      // TODO: Err handling
      NOM_STUBBED( NOM );
      return false;

      break;
    }

    case Value::ValueType::ObjectValues:
    {
      for( auto itr = object.begin(); itr != object.end(); ++itr )
      {
        ValueConstIterator member( itr );
        Value value = itr->ref();

        std::string object_key = member.key();
        NOM_DUMP( object_key );

        switch( value.type() )
        {
          default: // Unknown type
          {
            #if defined( NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES )
              NOM_DUMP( value.type() );
              NOM_DUMP( object_key );
              NOM_DUMP( value );
            #endif

            // TODO: Err handling
            NOM_STUBBED( NOM );
            return false;

            break;
          }

          case Value::ValueType::Null:
          case Value::ValueType::SignedInteger:
          case Value::ValueType::UnsignedInteger:
          case Value::ValueType::RealNumber:
          case Value::ValueType::String:
          case Value::ValueType::Boolean:
          {
            if( this->write_value( value, object_key, doc, parent ) == false )
            {
              // TODO: Err handling
              NOM_STUBBED( NOM );
              return false;
            }

            break;
          }

          // Handle arrays within an object within an object.
          case Value::ValueType::ArrayValues:
          {
            NOM_DUMP( object.type() );

            // FIXME:
            rapidxml::xml_node<>* val = doc.allocate_node( rapidxml::node_element, this->stralloc( object_key, doc ) );
            parent->append_node( val );

            if( this->serialize_array( value, object_key, doc, parent ) == false )
            {
              // TODO: Error message
              NOM_STUBBED( NOM );
              return false;
            }

            break;
          }

          // Handle iterating through objects within objects (unlimited depth).
          case Value::ValueType::ObjectValues:
          {
            rapidxml::xml_node<>* val = doc.allocate_node( rapidxml::node_element, this->stralloc( object_key, doc ) );
            parent->append_node( val );

            if( this->serialize_object( value, doc, val ) == false )
            {
              // TODO: Error message
              NOM_STUBBED( NOM );
              return false;
            }

            break;
          }

        } // end switch objects[key] type
      } // end for object loop
    } // end Value::ValueType::ObjectValues type
  } // end switch object type

  return true;
}

bool RapidXmlSerializer::read_value( const rapidxml::xml_node<>& object, Value& dest ) const
{
  NOM_STUBBED( NOM );

  return false;
}

bool RapidXmlSerializer::unserialize_array( const rapidxml::xml_node<>* object, Value& dest ) const
{
  std::string key;
  std::string value;
  Array arr;

  for( rapidxml::xml_attribute<> *attr = object->first_attribute(); attr; attr = attr->next_attribute() )
  {
    key = attr->name();
    value = attr->value();

    #if defined( NOMLIB_XML_RAPIDXML_UNSERIALIZER_HPP )
      NOM_DUMP( key );
      NOM_DUMP( value );
    #endif

    arr.push_back( attr->value() );
  }

  dest = arr;

  return true;
}

bool RapidXmlSerializer::unserialize_object( const rapidxml::xml_node<>* node, Value& dest ) const
{
  std::string key;
  std::string value;
  Object obj;
  Value arr;

  switch( node->type() )
  {
    default: // Unknown type
    {
      #if defined( NOM_DEBUG_RAPIDXML_UNSERIALIZER_VALUES )
        NOM_DUMP( node->type() );
      #endif

      // TODO: Err handling
      NOM_STUBBED( NOM );
      // return false;

      break;
    }

    case rapidxml::node_document:
    {
      // TODO: Handle case
      break;
    }

    case rapidxml::node_element:
    {
      for( rapidxml::xml_node<> *object = node->first_node(); object; object = object->next_sibling() )
      {
         key = object->name();
         value = object->value();

        if( key != "" )
        {
          #if defined( NOM_DEBUG_RAPIDXML_UNSERIALIZER_VALUES )
            NOM_DUMP(key);
          #endif

          this->unserialize_object( object, obj[key] );

          if( value != "" )
          {
            #if defined( NOM_DEBUG_RAPIDXML_UNSERIALIZER_VALUES )
               NOM_DUMP( value );
            #endif

             obj[key] = value;
          }

          this->unserialize_array( object, arr );

          if( arr.size() > 0 )
          {
            obj[key] = arr;
          }
        }
      }

      break;
    }

    case rapidxml::node_data:
    {
      // TODO: Handle case
      break;
    }

    case rapidxml::node_cdata:
    {
      // TODO: Handle case
      break;
    }

    case rapidxml::node_comment:
    {
      // TODO: Handle case
      break;
    }

    case rapidxml::node_declaration:
    {
      // TODO: Handle case
      break;
    }

    case rapidxml::node_doctype:
    {
      // TODO: Handle case
      break;
    }

    case rapidxml::node_pi:
    {
      // TODO: Handle case
      break;
    }
  }

  dest = obj;

  return true;
}

void RapidXmlSerializer::append_decl( rapidxml::xml_document<>& dest ) const
{
  // Top-level XML DOCTYPE
  rapidxml::xml_node<>* decl = dest.allocate_node( rapidxml::node_declaration );
  decl->append_attribute( dest.allocate_attribute("version", "1.0") );
  decl->append_attribute( dest.allocate_attribute("encoding", "utf-8") );
  dest.append_node( decl );
}

const char* RapidXmlSerializer::stralloc( const std::string& buffer, rapidxml::xml_document<>& dest ) const
{
  return dest.allocate_string( buffer.c_str() );
}

// std::ostream& operator <<( std::ostream& os, const Value& val )
// {
//   // os << val.dump( val );

//   os << "";

//   return os;
// }

} // namespace nom
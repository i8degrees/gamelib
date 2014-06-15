#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include <nomlib/serializers.hpp>
#include <nomlib/ptree.hpp> // Property Tree (nom::Value)

#include "serializers/common.hpp"

namespace nom {

/// \brief This unit test covers the serialization of Value objects to XML
// nodes.
class RapidXmlSerializerTest: public ::testing::Test
{
  public:
    /// \brief Default constructor; initialize our input/ouput interfaces to a
    /// sane state, suited for automated testing.
    ///
    /// \note Nearly every test fails without compact (no formatting) output.
    RapidXmlSerializerTest( void ) :
      fp{ new RapidXmlSerializer( nom::SerializerOptions::Compact ) },
      fp_in{ new RapidXmlDeserializer() },
      fp_outs{ new RapidXmlSerializer( nom::SerializerOptions::HumanFriendly ) }
    {
      // ...
    }

    ~RapidXmlSerializerTest( void )
    {
      delete this->fp;
      this->fp = nullptr;
    }

  protected:
    nom::IValueSerializer* fp;
    nom::IValueDeserializer* fp_in;
    nom::IValueSerializer* fp_outs;

    void expected_out( Value& in, const std::string& out )
    {
      std::string ret;

      ret = fp->serialize( in );

      EXPECT_EQ( out, ret ) << in;
    }

    void not_expected_out( Value& in, const std::string& out )
    {
      std::string ret;

      ret = fp->serialize( in );

      EXPECT_NE( out, ret ) << in;
    }

    std::string pp_out( const Value& in )
    {
      return fp->serialize( in );
    }

    std::string pp_outs( const Value& in )
    {
      return fp_outs->serialize( in );
    }
};

// FIXME?
TEST_F( RapidXmlSerializerTest, SerializeNullValue )
{
  Value o;

  expected_out( o, "" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeArrayValues_NullValue )
{
  Value o;

  o[0]["key"] = Value();

  expected_out( o, "" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeArrayValues_IntegerValue )
{
  Value o;

  o[0]["key"] = -1;

  expected_out( o, "" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeArrayValues_UsignedIntegerValue )
{
  Value o;
  o[0]["key"] = 1u;

  expected_out( o, "" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeArrayValues_RealNumberValue )
{
  Value o;
  o[0]["key1"] = 8.245;
  o[1]["key2"] = 8.24f;

  expected_out( o, "" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeArrayValues_StringValue )
{
  Value o;
  o[0]["key1"] = "Hello";
  o[1]["key2"] = "...there!";

  expected_out( o, "" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeArrayValues_BooleanValue )
{
  Value o;
  o[0]["key1"] = true;
  o[1]["key2"] = false;

  expected_out( o, "" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeArrayValues_ArrayValues )
{
  Value o;
  o[0]["arr"][0] = Value::null;
  o[0]["arr"][1] = "Yeah, buddy!";
  o[0]["arr"][2] = -8;
  o[0]["arr"][3] = 10u;
  o[0]["arr"][4] = false;

  expected_out( o, "" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeArrayValues_SimpleObjectValues )
{
  Value o;

  o[0]["id"] = 5u;
  o[0]["name"] = "Diablos";
  o[0]["level"] = 4u;
  o[0]["owner"] = false;

  expected_out( o, "" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeArrayValues_MultipleSimpleObjectValues )
{
  Value o;

  o[0]["id"] = 5u;
  o[0]["name"] = "Diablos";
  o[0]["level"] = 4u;
  o[0]["owner"] = false;

  o[1]["id"] = 99u;
  o[1]["name"] = "Squall";
  o[1]["level"] = 4u;
  o[1]["owner"] = Value();

  o[2]["id"] = 55u;
  o[2]["name"] = "Zell";
  o[2]["level"] = 2u;
  o[2]["owner"] = true;

  o[3] = Value();

  expected_out( o, "" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeArrayValues_ArrayValuesInObjectValues )
{
  Value o;

  o[0]["id"] = 5u;
  o[0]["name"] = "Diablos";
  o[0]["level"] = 4u;
  o[0]["ranks"][0] = 4u;
  o[0]["ranks"][1] = 8u;
  o[0]["ranks"][2] = 5u;
  o[0]["ranks"][3] = 10u;
  o[0]["owner"] = false;

  expected_out( o, "" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeArrayValues_MultipleArrayValuesInObjectValues )
{
  Value o;

  o[0]["id"] = 5u;
  o[0]["name"] = "Diablos";
  o[0]["level"] = 4u;
  o[0]["ranks"][0] = 4u;
  o[0]["ranks"][1] = 8u;
  o[0]["ranks"][2] = 5u;
  o[0]["ranks"][3] = 10u;
  o[0]["owner"] = false;

  o[1]["id"] = 0u;
  o[1]["name"] = "Geezard";
  o[1]["level"] = 1u;
  o[1]["ranks"][0] = 5u;
  o[1]["ranks"][1] = 3u;
  o[1]["ranks"][2] = 1u;
  o[1]["ranks"][3] = 3u;
  o[1]["owner"] = true;

  expected_out( o, "" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeArrayValues_ObjectInsideArrayValues )
{
  Value o, obj;

  obj[0]["id"] = 5u;
  obj[0]["name"] = "Diablos";
  obj[0]["level"] = 4u;
  obj[0]["ranks"][0] = 4u;
  obj[0]["ranks"][1] = 8u;
  obj[0]["ranks"][2] = 5u;
  obj[0]["ranks"][3] = 10u;
  obj[0]["owner"] = false;

  o[0]["cards"] = obj;

  obj[1]["id"] = 0u;
  obj[1]["name"] = "Geezard";
  obj[1]["level"] = 1u;
  obj[1]["ranks"][0] = 5u;
  obj[1]["ranks"][1] = 3u;
  obj[1]["ranks"][2] = 1u;
  obj[1]["ranks"][3] = 3u;
  obj[1]["owner"] = true;

  o[1]["cards"] = obj;

  expected_out( o, "" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeArrayValues_ComplexArrayValues )
{
  Value o;
  Value obj;
  Value arr;
  Value obj2;

  obj["name"] = "Geezard"; // Object node

  // Array node
  obj["ranks"][0] = 1u;
  obj["ranks"][1] = 5u;
  obj["ranks"][2] = 3u;
  obj["ranks"][3] = 1u;
  obj["ranks"][4] = 3u;
  obj["ranks"][5] = true;

  arr["__ranks"].push_back( obj ); // Array node

  obj2["name"] = "Geezard"; // Under an Object node
  obj2["__array"].push_back( arr ); // Array node

  o[0]["cards"] = obj2;

  expected_out( o, "" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeArrayValues_ComplexObjectValues )
{
  Value o;
  Value obj1;
  Value obj2;
  Value obj3;
  Value obj4;

  obj1["id"] = 0u;
  obj1["name"] = "Geezard";
  o[0]["cards"]["obj1"] = obj1;

  obj2["id"] = 1u;
  obj2["name"] = "Bite Bug";
  o[1]["cards"]["obj2"] = obj2;

  obj3["id"] = 2u;
  obj3["name"] = "MiniMog";
  o[2]["cards"]["obj3"]["obj4"] = obj3;

  obj4["id"] = 3u;
  obj4["name"] = "Chicobo";
  o[3]["cards"]["obj4"] = obj4;

  expected_out( o, "" );
}

TEST_F( RapidXmlSerializerTest, SerializeObjectValues_NullValue )
{
  Value o;

  o["root"]["obj"] = Value();

  expected_out( o, "<root><null/></root>" );
}

TEST_F( RapidXmlSerializerTest, SerializeObjectValues_IntegerValue )
{
  Value o;

  o["root"]["obj"] = -1;

  expected_out( o, "<root><obj>-1</obj></root>" );
}

TEST_F( RapidXmlSerializerTest, SerializeObjectValues_UsignedIntegerValue )
{
  Value o;
  o["root"]["obj"] = 1u;

  expected_out( o, "<root><obj>1</obj></root>" );
}

TEST_F( RapidXmlSerializerTest, SerializeObjectValues_RealNumberValue )
{
  Value o;
  o["root"]["obj1"] = 8.245;
  o["root"]["obj2"] = 8.240;

  expected_out( o, "<root><obj1>8.245000</obj1><obj2>8.240000</obj2></root>" );
}

TEST_F( RapidXmlSerializerTest, SerializeObjectValues_StringValue )
{
  Value o;
  o["root"]["obj"]["key1"] = "Hello";
  o["root"]["obj"]["key2"] = "...there!";

  expected_out( o, "<root><obj><key1>Hello</key1><key2>...there!</key2></obj></root>" );
}

TEST_F( RapidXmlSerializerTest, SerializeObjectValues_BooleanValue )
{
  Value o;
  o["obj"]["key1"] = true;
  o["obj"]["key2"] = false;

  expected_out( o, "<obj><key1>true</key1><key2>false</key2></obj>" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeObjectValues_ArrayValues )
{
  Value o;
  o["obj"]["arr"][0] = Value::null;
  o["obj"]["arr"][1] = "Yeah, buddy!";
  o["obj"]["arr"][2] = -8;
  o["obj"]["arr"][3] = 10u;
  o["obj"]["arr"][4] = false;

  expected_out( o, "<obj><arr/></obj>" );
}

TEST_F( RapidXmlSerializerTest, SerializeObjectValues_SimpleObjectValues )
{
  Value o;

  o["obj"]["id"] = 5u;
  o["obj"]["name"] = "Diablos";
  o["obj"]["level"] = 4u;
  o["obj"]["owner"] = false;

  expected_out( o, "<obj><id>5</id><level>4</level><name>Diablos</name><owner>false</owner></obj>" );
}

TEST_F( RapidXmlSerializerTest, SerializeObjectValues_MultipleSimpleObjectValues )
{
  Value o;

  o["obj1"]["id"] = 5u;
  o["obj1"]["name"] = "Diablos";
  o["obj1"]["level"] = 4u;
  o["obj1"]["owner"] = false;

  o["obj2"]["id"] = 99u;
  o["obj2"]["name"] = "Squall";
  o["obj2"]["level"] = 4u;
  o["obj2"]["owner"] = Value();

  o["obj3"]["id"] = 55u;
  o["obj3"]["name"] = "Zell";
  o["obj3"]["level"] = 2u;
  o["obj3"]["owner"] = true;

  o["obj4"] = Value();

  expected_out( o, "<obj1><id>5</id><level>4</level><name>Diablos</name><owner>false</owner></obj1><obj2><id>99</id><level>4</level><name>Squall</name><null/></obj2><obj3><id>55</id><level>2</level><name>Zell</name><owner>true</owner></obj3><obj4><null/></obj4>" );
}

// TODO: Implement ArrayValues support
TEST_F( RapidXmlSerializerTest, SerializeObjectValues_ArrayValuesInObjectValues )
{
  Value o;

  o["obj"]["id"] = 5u;
  o["obj"]["name"] = "Diablos";
  o["obj"]["level"] = 4u;
  o["obj"]["ranks"][0] = 4u;
  o["obj"]["ranks"][1] = 8u;
  o["obj"]["ranks"][2] = 5u;
  o["obj"]["ranks"][3] = 10u;
  o["obj"]["owner"] = false;

  expected_out( o, "<obj><id>5</id><level>4</level><name>Diablos</name><owner>false</owner><ranks/></obj>" );
}

TEST_F( RapidXmlSerializerTest, SerializeObjectValues_MultipleArrayValuesInObjectValues )
{
  Value o;

  o["obj1"]["id"] = 5u;
  o["obj1"]["name"] = "Diablos";
  o["obj1"]["level"] = 4u;
  o["obj1"]["ranks"][0] = 4u;
  o["obj1"]["ranks"][1] = 8u;
  o["obj1"]["ranks"][2] = 5u;
  o["obj1"]["ranks"][3] = 10u;
  o["obj1"]["owner"] = false;

  o["obj2"]["id"] = 0u;
  o["obj2"]["name"] = "Geezard";
  o["obj2"]["level"] = 1u;
  o["obj2"]["ranks"][0] = 5u;
  o["obj2"]["ranks"][1] = 3u;
  o["obj2"]["ranks"][2] = 1u;
  o["obj2"]["ranks"][3] = 3u;
  o["obj2"]["owner"] = true;

  expected_out( o, "<obj1><id>5</id><level>4</level><name>Diablos</name><owner>false</owner><ranks/></obj1><obj2><id>0</id><level>1</level><name>Geezard</name><owner>true</owner><ranks/></obj2>" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeObjectValues_ObjectInsideArrayValues )
{
  Value o, obj;

  obj["obj1"]["id"] = 5u;
  obj["obj1"]["name"] = "Diablos";
  obj["obj1"]["level"] = 4u;
  obj["obj1"]["ranks"][0] = 4u;
  obj["obj1"]["ranks"][1] = 8u;
  obj["obj1"]["ranks"][2] = 5u;
  obj["obj1"]["ranks"][3] = 10u;
  obj["obj1"]["owner"] = false;

  o["root"]["cards"].push_back( obj );

  obj["obj2"]["id"] = 0u;
  obj["obj2"]["name"] = "Geezard";
  obj["obj2"]["level"] = 1u;
  obj["obj2"]["ranks"][0] = 5u;
  obj["obj2"]["ranks"][1] = 3u;
  obj["obj2"]["ranks"][2] = 1u;
  obj["obj2"]["ranks"][3] = 3u;
  obj["obj2"]["owner"] = true;

  o["root"]["cards"].push_back( obj );

  expected_out( o, "<root><cards/></root>" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeObjectValues_ComplexArrayValues )
{
  Value o;
  Value obj;
  Value arr;
  Value obj2;

  obj["name"] = "Geezard"; // Object node

  // Array node
  obj["ranks"][0] = 1u;
  obj["ranks"][1] = 5u;
  obj["ranks"][2] = 3u;
  obj["ranks"][3] = 1u;
  obj["ranks"][4] = 3u;
  obj["ranks"][5] = true;

  arr["__ranks"].push_back( obj ); // Array node

  obj2["name"] = "Geezard"; // Under an Object node
  obj2["__array"].push_back( arr ); // Array node

  o["root"]["cards"] = obj2;

  expected_out( o, "<root><cards><__array/><name>Geezard</name></cards></root>" );
}

// TODO
TEST_F( RapidXmlSerializerTest, SerializeObjectValues_ComplexObjectValues )
{
  Value o;
  Value obj1;
  Value obj2;
  Value obj3;
  Value obj4;

  obj1["id"] = 0u;
  obj1["name"] = "Geezard";
  o["root"]["cards"]["obj1"] = obj1;

  obj2["id"] = 1u;
  obj2["name"] = "Bite Bug";
  o["root"]["cards"]["obj2"] = obj2;

  obj3["id"] = 2u;
  obj3["name"] = "MiniMog";
  o["root"]["cards"]["obj3"]["obj4"] = obj3;

  obj4["id"] = 3u;
  obj4["name"] = "Chicobo";
  o["root"]["cards"]["obj4"] = obj4;

  expected_out( o, "<root><cards><obj1><id>0</id><name>Geezard</name></obj1><obj2><id>1</id><name>Bite Bug</name></obj2><obj3><obj4><id>2</id><name>MiniMog</name></obj4></obj3><obj4><id>3</id><name>Chicobo</name></obj4></cards></root>" );
}

TEST_F( RapidXmlSerializerTest, SerializeObjectValuesSanity2 )
{
  Value o;

  o = sanity2_out();

  expected_out( o, "<object1><boolean1>false</boolean1><null/><response1>Yeah buddy!</response1></object1><object2><array2/><boolean2>true</boolean2><deep_object><h><boolean>false</boolean><deep_deep_deep_object><deep_deep_deep_array/><h>240</h><w>420</w></deep_deep_deep_object><key>pair</key><null/></h><w>-4</w></deep_object><response2>Light weight!</response2></object2><object3/><object5/>" );
}

TEST_F( RapidXmlSerializerTest, SerializeXMLTestDocumentOne )
{
  Value o;

  o["sheet"]["cursors.png"]["ID_0"]["dims"]["x"] = 0;
  o["sheet"]["cursors.png"]["ID_0"]["dims"]["y"] = 0;
  o["sheet"]["cursors.png"]["ID_0"]["dims"]["w"] = 26;
  o["sheet"]["cursors.png"]["ID_0"]["dims"]["h"] = 16;

  o["sheet"]["cursors.png"]["ID_1"]["dims"]["x"] = 26;
  o["sheet"]["cursors.png"]["ID_1"]["dims"]["y"] = 0;
  o["sheet"]["cursors.png"]["ID_1"]["dims"]["w"] = 26;
  o["sheet"]["cursors.png"]["ID_1"]["dims"]["h"] = 16;

  o["sheet"]["cursors.png"]["meta"]["scale"] = 1;
  o["sheet"]["cursors.png"]["meta"]["format"] = "RGBA8888";
  o["sheet"]["cursors.png"]["meta"]["spacing"] = 0;
  o["sheet"]["cursors.png"]["meta"]["padding"] = 0;
  o["sheet"]["cursors.png"]["meta"]["version"] = "0.3.0";
  o["sheet"]["cursors.png"]["meta"]["frames"] = 5u;
  o["sheet"]["cursors.png"]["meta"]["sha1sum"] = "fbb26a620528a062ba43ccb4bb5e71c714df8e5e";
  o["sheet"]["cursors.png"]["meta"]["creator"] = "nomlib";
  o["sheet"]["cursors.png"]["meta"]["size"][0] = 130;
  o["sheet"]["cursors.png"]["meta"]["size"][1] = 16;

  o["sheet"]["faces.png"]["ID_0"]["dims"]["x"] = 1;
  o["sheet"]["faces.png"]["ID_0"]["dims"]["y"] = 1;
  o["sheet"]["faces.png"]["ID_0"]["dims"]["w"] = 64;
  o["sheet"]["faces.png"]["ID_0"]["dims"]["h"] = 64;

  o["sheet"]["faces.png"]["ID_1"]["dims"]["x"] = 1;
  o["sheet"]["faces.png"]["ID_1"]["dims"]["y"] = 64;
  o["sheet"]["faces.png"]["ID_1"]["dims"]["w"] = 64;
  o["sheet"]["faces.png"]["ID_1"]["dims"]["h"] = 64;

  o["sheet"]["faces.png"]["meta"]["scale"] = 1;
  o["sheet"]["faces.png"]["meta"]["format"] = "RGBA8888";
  o["sheet"]["faces.png"]["meta"]["spacing"] = 1;
  o["sheet"]["faces.png"]["meta"]["padding"] = 1;
  o["sheet"]["faces.png"]["meta"]["version"] = "0.3.0";
  o["sheet"]["faces.png"]["meta"]["frames"] = 110u;
  o["sheet"]["faces.png"]["meta"]["sha1sum"] = "fbb26a620528a062ba43ccb4bb5e71c714df8e5e";
  o["sheet"]["faces.png"]["meta"]["creator"] = "nomlib";
  o["sheet"]["faces.png"]["meta"]["size"][0] = 2082;
  o["sheet"]["faces.png"]["meta"]["size"][1] = 262;

// FAIL() << pp_out(o);
}

TEST_F( RapidXmlSerializerTest, DISABLED_FileIO )
{
  Value in;
  Value o;

  o = sanity2_out();

  ASSERT_TRUE( fp->save( o, "output_human-readable.json" ) ) << o;

  // Just to be safe!
  o.clear();
  o = sanity2_out();

  ASSERT_TRUE( fp->save( o, "output_compact.json" ) ) << o;

  // Attempt to verify that the object we serialized is, in fact, the same data
  // representation as what we are able to retrieve.
  ASSERT_TRUE( fp_in->load( "output_human-readable.json", in ) ) << in;

  ASSERT_TRUE( in == o ) << in;

  // Just to be safe!
  in.clear();

  ASSERT_TRUE( fp_in->load( "output_compact.json", in ) ) << in;

  ASSERT_TRUE( in == o ) << in;
}

// FIXME: Implement ArrayValues support in our XML interface
TEST_F( RapidXmlSerializerTest, DISABLED_ExtraIO )
{
  nom::IValueDeserializer* json = new nom::JsonCppDeserializer();

  Value in1, in2;
  Value os1, os2;

  // Un-serialize the JSON data into Value objects
  ASSERT_TRUE( json->load( RESOURCE_JSON_AUCTIONS, os1 ) );
  ASSERT_TRUE( json->load( RESOURCE_JSON_INVENTORY, os2 ) );

  // Re-serialize our Value objects back as XML
  ASSERT_TRUE( fp->save( in1, "ah.xml" ) );
  ASSERT_TRUE( fp->save( in2, "inv.xml" ) );

  EXPECT_EQ( in1, os1 );
  EXPECT_EQ( in2, os2 );

  // FIXME: This will load for us only if we use Json::Reader directly to read.
  // ASSERT_TRUE( fp->load( RESOURCE_JSON_GAMEDATA, os ) );
  // EXPECT_EQ( 4, os.size() );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}

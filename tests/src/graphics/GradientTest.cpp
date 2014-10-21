#include <iostream>
#include <string>

#include "gtest/gtest.h"

// nom::UnitTest framework
#include "nomlib/tests/common.hpp"

#include <nomlib/config.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>

// #if ! defined( NOM_USE_SCALEX )
//   #pragma message("NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT should be enabled for GradientTest::Margins")
// #endif

namespace nom {

class GradientTest: public nom::VisualUnitTest
{
  public:
    /// \remarks This method is called at the start of each unit test.
    GradientTest()
    {
      // NOM_LOG_TRACE( NOM );

      this->set_window_flags( SDL_WINDOW_RESIZABLE );

      // The frame image to compare against the reference image set
      this->append_screenshot_frame( 0 );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~GradientTest()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp()
    {
      // NOM_LOG_TRACE( NOM );

      std::string res_file = nom::UnitTest::test_set() + ".json";

      if( resources.load_file( res_file, "resources" ) == false )
      {
        FAIL()
        << "Could not resolve the resource path from file: " << res_file;
      }

      // VisualUnitTest environment init...
      VisualUnitTest::SetUp();

      EventCallback debug_info( [&] ( const Event& evt )
        {
          NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "x:", evt.mouse.x, "y:", evt.mouse.y );
        }
      );

      InputActionMapper state;

      state.insert( "debug_info", nom::MouseButtonAction(
                    SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT ),
                    debug_info );

      this->input_mapper_.insert( "debug_info", state, true );

      this->colors[0] = {
                          Color4iColors{
                            Color4i::Gray,
                            Color4i::LightGray
                          }
                        };
      this->colors[1] = {
                          Color4iColors{
                            nom::Color4i( 114, 66, 66 ),
                            nom::Color4i( 251, 222, 232 )
                          }
                        };

      int w = this->resolution().w;
      int h = this->resolution().h;
      this->pos1 = Point2i( 0, 0 );
      this->dims1 = Size2i( w, h / 2 );

      this->pos2 = Point2i( 0, h / 2 );
      this->dims2 = Size2i( w, h / 2 );

      // Not used
      this->pos3 = Point2i( w/3, h/2 );
      this->dims3 = Size2i( w/3, h/3 );

      this->grad1.set_colors( this->colors[0] );
      this->grad1.set_position( this->pos1 );
      this->grad1.set_size( this->dims1 );

      this->grad2.set_colors( this->colors[1] );
      this->grad2.set_position( this->pos2 );
      this->grad2.set_size( this->dims2 );

      // Not used
      this->grad3.set_colors( this->colors[0] );
      this->grad3.set_position( this->pos3 );
      this->grad3.set_size( this->dims3 );

      // Register our main loop
      this->append_render_callback( [&] ( const RenderWindow& win ) { this->grad1.draw( this->render_window() ); } );
      this->append_render_callback( [&] ( const RenderWindow& win ) { this->grad2.draw( this->render_window() ); } );
      // this->append_render_callback( [&] ( const RenderWindow& win ) { this->grad3.draw( this->render_window() ); } );
    }

    /// \remarks This method is called before destruction, at the end of each
    /// unit test.
    virtual void TearDown()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called at the start of each test case.
    static void SetUpTestCase()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called at the end of each test case.
    static void TearDownTestCase()
    {
      // NOM_LOG_TRACE( NOM );
    }

  protected:
    SearchPath resources;

    Gradient grad1;
    Gradient grad2;
    // Not used
    Gradient grad3;

    Point2i pos1;
    Size2i dims1;

    Point2i pos2;
    Size2i dims2;

    // Not used
    Point2i pos3;
    Size2i dims3;

    /// \brief Colors used to create the gradient

    /// \remarks One of two used
    Color4iColors colors[2];
};

TEST_F( GradientTest, TopToBottomLinearFill )
{
  this->grad1.set_fill_direction( Gradient::FillDirection::Top );
  this->grad2.set_fill_direction( Gradient::FillDirection::Top );
  // this->grad3.set_fill_direction( Gradient::FillDirection::Top );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( GradientTest, BottomToTopLinearFill )
{
  this->grad1.set_fill_direction( Gradient::FillDirection::Bottom );
  this->grad2.set_fill_direction( Gradient::FillDirection::Bottom );
  // this->grad3.set_fill_direction( Gradient::FillDirection::Bottom );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( GradientTest, LeftToRightLinearFill )
{
  this->grad1.set_fill_direction( Gradient::FillDirection::Left );
  this->grad2.set_fill_direction( Gradient::FillDirection::Left );
  // this->grad3.set_fill_direction( Gradient::FillDirection::Left );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( GradientTest, RightToLeftLinearFill )
{
  this->grad1.set_fill_direction( Gradient::FillDirection::Right );
  this->grad2.set_fill_direction( Gradient::FillDirection::Right );
  // this->grad3.set_fill_direction( Gradient::FillDirection::Right );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

// TOOD: Verify / fix margins calculations for left to right directions
TEST_F( GradientTest, Margins )
{
  std::shared_ptr<Gradient> card_background;
  std::shared_ptr<SpriteBatch> card_face;

  // Loosely emulate TTcards scale resolution (scale up)
  const std::string sheet_coords = resources.path()+"faces.json";
  const std::string sheet_img = resources.path()+"faces.png";
  const int SCALE_FACTOR = 2;
  const int CARD_WIDTH = 64;
  const int CARD_HEIGHT = 64;
  const int CARD_ORIGIN_X =
    ( (this->resolution().w / SCALE_FACTOR) - CARD_WIDTH ) / 2;
  const int CARD_ORIGIN_Y =
    ( (this->resolution().h / SCALE_FACTOR) - CARD_HEIGHT ) / 2;

  Color4iColors player1_grad_bg = { Color4i( 208, 223, 255 ),
                                    Color4i( 50, 59, 114 )
                                  };
  Size2i dims;
  dims.w = this->resolution().w;
  dims.h = this->resolution().h;
  this->render_window().set_logical_size( dims );
  this->render_window().set_scale( Point2f(2,2) );

  card_background = std::make_shared<Gradient>( Gradient() );

  card_face =
    std::make_shared<SpriteBatch>( SpriteBatch( sheet_coords ) );

  if( card_face->load( sheet_img ) == false )
  {
    FAIL() << "Could not load image from file: " << sheet_img;
  }
  // card_face->resize( nom::Texture::ResizeAlgorithm::scale2x );

  // Geezard
  card_face->set_frame( 0 );

  card_face->set_position( Point2i(CARD_ORIGIN_X, CARD_ORIGIN_Y) );
  card_background->set_position( nom::Point2i(CARD_ORIGIN_X, CARD_ORIGIN_Y) );

  // The gradient should be bound within the
  //
  // Note that the 1 pixel border surrounding each card is why these
  // calculations are necessary; we want the gradient to be inside the card's
  // border
  card_background->set_size( Size2i(CARD_WIDTH-2, CARD_HEIGHT-2) );
  card_background->set_margins( Point2i(1,1) );
  card_background->set_colors( player1_grad_bg );
  card_background->set_fill_direction( nom::Gradient::FillDirection::Top );

  // 62, 62
  // NOM_DUMP( card_background->size() );
  // 128, 88
  // NOM_DUMP( card_background->position() );

  // Need to first clear out the default grad objects in here
  this->clear_render_callbacks();

  // Re-add the default rendering callback
  this->append_render_callback( this->default_render_callback() );

  this->append_render_callback( [&] ( const RenderWindow& win ) {
    card_background->draw( this->render_window() );
  });

  this->append_render_callback( [&] ( const RenderWindow& win ) {
    card_face->draw ( this->render_window() );
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  if( nom::init( argc, argv ) == false )
  {
    nom::DialogMessageBox( "Critical Error", "Could not initialize nomlib.", nom::MessageBoxType::NOM_DIALOG_ERROR );
    return NOM_EXIT_FAILURE;
  }
  atexit( nom::quit );

  // nom::UnitTest framework integration
  nom::init_test( argc, argv );

  nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_RENDER, nom::NOM_LOG_PRIORITY_VERBOSE );
  // nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  return RUN_ALL_TESTS();
}

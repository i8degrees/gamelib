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
#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "common.hpp"

#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>

namespace nom {

/// \brief Relative filename path to saved screen shots.
///
/// Default path should resolve to the same directory as the output binary.
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

/// \note The resources and variables used derived from examples/gui_button.cpp
/// and tests/gui/ListBoxLayoutTest.cpp.
class ButtonLayoutTest: public ::testing::Test
{
  public:
    /// \remarks Initialization of resources.
    ButtonLayoutTest( void ) :
      app_state{ true }
    {
      this->main_window = nullptr;

      nom::init_third_party( InitHints::DefaultInit );

      if( nom::set_hint( SDL_HINT_RENDER_VSYNC, "0" ) == false )
      {
        NOM_LOG_INFO( NOM, "Could not disable vertical refresh." );
      }

      // this->input_mapper.clear();

      InputActionMapper state;
      KeyboardAction action;

      EventCallback ev_quit( [&] () { this->app_state = false; } );
      EventCallback ev_screenshot( [&] () { this->window.save_screenshot( OUTPUT_SCREENSHOT_FILENAME ); } );
      EventCallback ev_fullscreen( [&] () { this->window.toggle_fullscreen(); } );

      action = nom::KeyboardAction( SDL_KEYDOWN, SDLK_ESCAPE );
      state.insert( "ev_quit", nom::InputAction( action, ev_quit ) );

      action = nom::KeyboardAction( SDL_KEYDOWN, SDLK_q );
      state.insert( "ev_quit", nom::InputAction( action, ev_quit ) );

      action = nom::KeyboardAction( SDL_KEYDOWN, SDLK_F1 );
      state.insert( "ev_screenshot", nom::InputAction( action, ev_screenshot ) );

      action = nom::KeyboardAction( SDL_KEYDOWN, SDLK_f );
      state.insert( "ev_fullscreen", nom::InputAction( action, ev_fullscreen ) );

      this->input_mapper.insert( "ButtonLayoutTest", state, true );
    }

    virtual ~ButtonLayoutTest( void )
    {
      // Nothing to clean up
    }

    /// \remarks This method is called at the start of each unit test.
    virtual void SetUp( void )
    {
      nom::uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;

      // Necessary for loading font resources
      ASSERT_TRUE( this->window.create( "ButtonLayoutTest", WINDOW_WIDTH, WINDOW_HEIGHT, window_flags ) );

      // Scale window contents up by the new width & height
      this->window.set_logical_size( WINDOW_WIDTH, WINDOW_HEIGHT );

      // BitmapButton (button2) resources
      ASSERT_TRUE( this->button_bg[0].load( RESOURCE_BUTTON_IMAGE, 0 ) == true );
      ASSERT_TRUE( this->button_bg[1].load( RESOURCE_AQUA_BUTTON_IMAGE_0, 0 ) == true );
      ASSERT_TRUE( this->button_bg[2].load( RESOURCE_AQUA_BUTTON_IMAGE_1, 0 ) == true );

      // Top-level (parent) window (relative to global "screen" coordinates):
      this->main_window = new nom::UIWidget( Point2i( 25, 25 ), Size2i( WINDOW_WIDTH - 100, WINDOW_HEIGHT / 2 ) );

      // TODO:
      // this->main_window = new nom::UIWidget( Point2i( 25, 25 ), Size2i( WINDOW_WIDTH - 100, 80 ) );

      this->main_window->set_name( "Layout" );
      this->main_window->set_title( this->main_window->name() );

      // Draw a frame so that we can visually see the maximal bounds of the
      // layout
      this->main_window->set_decorator( new nom::MinimalDecorator() );

      // FIXME: R-enable the underlying event code in UIWidget for the
      // WINDOW_MOUSE_DOWN event.
      //
      // this->main_window->register_event_listener( nom::UIEvent::WINDOW_MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->window_on_click( ev ); } ) );

      // Our widgets to be used in the layout:

      // Note that each button's size (other than the bitmap button) will be
      // too small to fit the text used (which was intentional), but with layout
      // management, the size dimensions will be calculated for us automatically
      // (meaning that the size dimensions used will be from the widget's
      // size_hint method, when the size policy is set to Preferred).

      // this->button0 = this->create_button( this->main_window, Point2i::null, Size2i(50,16), "button0", "Click me!" );
      this->button0 = this->create_button( this->main_window, Point2i::null, Size2i::null, "button0", "Click me!" );

      this->button0->set_font( SystemFonts::cache().load_resource("VIII") );
      this->button0->set_decorator( new nom::FinalFantasyDecorator() );
      // this->button0->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->button0_on_click( ev ); } ) );
      // this->button0->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->button0_on_click( ev ); } ) );

      // this->button1 = this->create_button( this->main_window, Point2i::null, Size2i::null, "button1", "button1" );

      // Create a button using explicit size dimensions; this internally sets
      // the size policy to use these dimensions within a layout.
      this->button1 = this->create_button( this->main_window, Point2i::null, Size2i(56,16), "button1", "button1" );

      this->button1->set_decorator( new nom::MinimalDecorator() );
      // this->button1->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->button1_on_click( ev ); } ) );
      // this->button1->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->button1_on_click( ev ); } ) );

      // Style is used for button2
      nom::UIStyle::shared_ptr custom_style = nullptr;
      custom_style.reset( new nom::UIStyle() );

      custom_style->set_font_color( nom::Color4i::Black );
      custom_style->set_font_size( 13 );
      custom_style->set_font_style( nom::Text::Style::Bold );

      // this->button2 = this->create_bitmap_button( this->main_window, Point2i::null, Size2i(50,16), "bitmap_button", "Hello", this->button_bg[1], custom_style );
      this->button2 = this->create_bitmap_button( this->main_window, Point2i::null, Size2i::null, "bitmap_button", "Hello", this->button_bg[1], custom_style );

      this->button2->set_pressed_bitmap( this->button_bg[2] );
      // this->button2->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { priv::on_click( ev ); } ) );
      // this->button2->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { priv::on_click( ev ); } ) );

      // this->button3 = this->create_button( this->main_window, Point2i::null, Size2i::null, "button3", "button3" );
      this->button3 = this->create_button( this->main_window, Point2i::null, Size2i(56,16), "button3", "button3" );
      this->button3->set_decorator( new nom::MinimalDecorator() );

      this->main_window->insert_child( this->button0 );
      this->main_window->insert_child( this->button1 );
      this->main_window->insert_child( this->button2 );
      this->main_window->insert_child( this->button3 );

      this->spacers.push_back( 8 );
      this->items.push_back( this->button0 );
      this->spacers.push_back( 8 );
      this->items.push_back( this->button1 );
      this->spacers.push_back( 40 );
      this->items.push_back( this->button2 );
      this->spacers.push_back( 2 );
      this->items.push_back( this->button3 );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual void TearDown( void )
    {
      // Note that the top-level (parent) UIWidget is the owner of its children,
      // thus it relieves us from the responsibility of freeing them --
      // button0, button1, button2.
      delete this->main_window;
      this->main_window = nullptr;
    }

    sint on_run( void )
    {
      while( this->app_state == true )
      {
        while( this->evt.poll_event( this->ev ) )
        {
          // this->evt.process_event( this->ev );

          this->input_mapper.on_event( this->ev );
          this->main_window->process_event( this->ev );
        }

        this->window.update();
        this->main_window->update();

        // Background color fill
        this->window.fill( nom::Color4i::SkyBlue );

        this->main_window->draw( this->window );
      }

      return NOM_EXIT_SUCCESS;
    }

    /// \brief Helper method for creating button widgets.
    nom::Button::raw_ptr create_button( const UIWidget::raw_ptr window, const Point2i& pos, const Size2i& size, const std::string& name, const std::string& label )
    {
      nom::Button::raw_ptr button = new nom::Button( window, -1, pos, size );
      // nom::Button::raw_ptr button = new nom::Button( window );

      button->set_name( name );
      NOM_ASSERT( button->name() == name );

      button->set_label( label );
      NOM_ASSERT( button->label_text() == label );

      NOM_ASSERT( button->parent()->position() == window->position() );

      // NOM_DUMP( button->name() );
      // NOM_DUMP( size );           // layout dims (?)
      // NOM_DUMP( button->size() ); // ::size_hint dims (correct)

      NOM_ASSERT( button->size() == size );

      NOM_ASSERT( button->font().valid() );

      return button;
    }

    /// \brief Helper method for creating bitmap button widgets.
    nom::BitmapButton::raw_ptr create_bitmap_button( const UIWidget::raw_ptr window, const Point2i& pos, const Size2i& size, const std::string& name, const std::string& label, const Texture& tex, const UIStyle::shared_ptr style )
    {
      // TODO: Create separate examples (including a nullptr test):
      // button = new nom::BitmapButton( window, pos, size, nullptr );
      // button = new nom::BitmapButton( this->main_window, -1, pos, size, &this->button_bg[0] );
      // button = new nom::BitmapButton( this->main_window, -1, pos, size, &this->button_bg[0] );

      nom::BitmapButton::raw_ptr button = new nom::BitmapButton( window, -1, pos, size, tex );
      // nom::BitmapButton::raw_ptr button = new nom::BitmapButton( window, tex );

      button->set_name( name );
      NOM_ASSERT( button->name() == name );

      // FIXME: This has to be set before the text label; this is the same
      // problem as we had with nom::Text before relocating everything to
      // ::update.
      button->set_style( style );

      button->set_label( label );
      NOM_ASSERT( button->label_text() == label );

      NOM_ASSERT( button->parent()->position() == window->position() );

      // NOM_DUMP( button->name() );
      // NOM_DUMP(size);           // layout dims (?)
      // NOM_DUMP(button->size()); // ::size_hint dims (correct)

      NOM_ASSERT( button->size() == button->bitmap_size() );

      NOM_ASSERT( button->font().valid() );

      return button;
    }

    /// \brief Alternative callback method used in registering event listeners.
    void button0_on_click( const nom::UIWidgetEvent& ev ) const
    {
      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );

      NOM_ASSERT( this->button0 != nullptr );

      if( ev.index() == 0 ) // MOUSE_DOWN
      {
        this->button0->set_label( "Click me!" );
      }
      else if( ev.index() == 1 ) // MOUSE_UP
      {
        this->button0->set_label( "Release me!" );
      }
    }

    /// \brief Alternative callback method used in registering event listeners.
    void button1_on_click( const nom::UIWidgetEvent& ev ) const
    {
      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );

      NOM_ASSERT( this->button1 != nullptr );

      if( ev.index() == 0 ) // MOUSE_DOWN
      {
        this->button1->set_label( "button1" );
      }
      else if( ev.index() == 1 ) // MOUSE_UP
      {
        this->button1->set_label( "Yeah buddy!" );
      }
    }

  protected:
    const nom::Path p;

    /// \brief File path name of the resources directory; this must be a relative file path.
    const std::string APP_RESOURCES_DIR = "Resources" + p.native();
    const std::string RESOURCE_BUTTON_IMAGE = APP_RESOURCES_DIR + p.native() + "gui" + p.native() + "button.png";
    const std::string RESOURCE_AQUA_BUTTON_IMAGE_0 = APP_RESOURCES_DIR + p.native() + "gui" + p.native() + "aqua-button_0.png";
    const std::string RESOURCE_AQUA_BUTTON_IMAGE_1 = APP_RESOURCES_DIR + p.native() + "gui" + p.native() + "aqua-button_1.png";

    const int WINDOW_WIDTH = 640;
    const int WINDOW_HEIGHT = 480;

    // Game loop support
    bool app_state;
    RenderWindow window;
    Event ev;

    EventHandler evt;
    InputStateMapper input_mapper;

    // GUI resources
    nom::UIWidget::raw_ptr main_window;

    /// \brief Image resource for our button widget.
    nom::Texture button_bg[3];

    nom::Button::raw_ptr button0;
    nom::Button::raw_ptr button1;
    nom::BitmapButton::raw_ptr button2;
    nom::Button::raw_ptr button3;

    /// \brief Expected position results (per test).
    std::vector<Point2i> pos;

    /// \brief Expected with and height results (per test).
    std::vector<Size2i> dims;

    /// \brief The widget items to test in a layout (per test).
    ///
    /// \remarks The stored pointers are not owned by us; do *not* free them!
    std::vector<UIWidget*> items;

    /// \brief The width and height of the UISpacerItem items to test in a
    /// layout (per test).
    std::vector<int> spacers;
};

TEST_F( ButtonLayoutTest, HorizontalLayout )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  // Use custom, absolute dimensions that we provide, with relative positioning
  // of the layout from the parent widget's coordinates.
  layout = priv::create_layout( this->main_window, this->items, this->spacers, Point2i(12,25), Size2i( WINDOW_WIDTH - 100, WINDOW_HEIGHT / 2 ), "HorizontalLayout", Orientations::Horizontal );

  EXPECT_EQ( this->items.size() + this->spacers.size(), layout->count() );

  EXPECT_EQ( Size2i( 56, 16 ), layout->minimum_size() );

  this->pos.push_back( Point2i( 45, 50 ) );
  this->dims.push_back( Size2i( 115, 30 ) );

  this->pos.push_back( Point2i( 384, 50 ) );
  this->dims.push_back( Size2i( 59, 16 ) );

  priv::expected_layout_spacer_output( layout, 0, Point2i(-1,-1), Size2i(8,8) );
  priv::expected_layout_output( layout, this->pos, this->dims );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F( ButtonLayoutTest, VerticalLayout )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  // Use custom, absolute dimensions that we provide, with relative positioning
  // of the layout from the parent widget's coordinates.
  layout = priv::create_layout( this->main_window, this->items, this->spacers, Point2i(12,25), Size2i( WINDOW_WIDTH - 100, WINDOW_HEIGHT / 2 ), "VerticalLayout", Orientations::Vertical );

  EXPECT_EQ( this->items.size() + this->spacers.size(), layout->count() );

  EXPECT_EQ( Size2i( 56, 16 ), layout->minimum_size() );

  this->pos.push_back( Point2i( 37, 58 ) );
  this->dims.push_back( Size2i( 112, 33 ) );

  this->pos.push_back( Point2i( 37, 193 ) );
  this->dims.push_back( Size2i( 56, 19 ) );

  priv::expected_layout_spacer_output( layout, 0, Point2i(-1,-1), Size2i(8,8) );
  priv::expected_layout_output( layout, this->pos, this->dims );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

/// \remarks This unit test is intended to be non-interactive. To enable
/// interactive running of the test, uncomment the last line of the unit test.
TEST_F( ButtonLayoutTest, NullSizeHorizontalLayout )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  // Use the dimensions that the parent widget provides, with relative
  // positioning of the layout from the parent widget's coordinates.
  layout = priv::create_layout( this->main_window, this->items, this->spacers, Point2i(12,25), Size2i::null, "NullSizeHorizontalLayout", Orientations::Horizontal );

  EXPECT_EQ( this->items.size() + this->spacers.size(), layout->count() );

  EXPECT_EQ( Size2i( 56, 16 ), layout->minimum_size() );

  this->pos.push_back( Point2i( 45, 50 ) );
  this->dims.push_back( Size2i( 115, 30 ) );

  this->pos.push_back( Point2i( 384, 50 ) );
  this->dims.push_back( Size2i( 59, 16 ) );

  priv::expected_layout_spacer_output( layout, 0, Point2i(-1,-1), Size2i(8,8) );
  priv::expected_layout_output( layout, pos, dims );

  // EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

/// \remarks This unit test is intended to be non-interactive. To enable
/// interactive running of the test, uncomment the last line of the unit test.
TEST_F( ButtonLayoutTest, NullSizeVerticalLayout )
{
  nom::UIBoxLayout::raw_ptr layout = nullptr;

  // Use the dimensions that the parent widget provides, with relative
  // positioning of the layout from the parent widget's coordinates.
  layout = priv::create_layout( this->main_window, this->items, this->spacers, Point2i(12,25), Size2i::null, "NullSizeVerticalLayout", Orientations::Vertical );

  EXPECT_EQ( this->items.size() + this->spacers.size(), layout->count() );

  EXPECT_EQ( Size2i( 56, 16 ), layout->minimum_size() );

  this->pos.push_back( Point2i( 37, 58 ) );
  this->dims.push_back( Size2i( 112, 33 ) );

  this->pos.push_back( Point2i( 37, 193 ) );
  this->dims.push_back( Size2i( 56, 19 ) );

  priv::expected_layout_spacer_output( layout, 0, Point2i(-1,-1), Size2i(8,8) );
  priv::expected_layout_output( layout, this->pos, this->dims );

  // EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

} // namespace nom

int main( int argc, char** argv )
{
  // Only used for setting the working directory path to that of the executable,
  // so we can quickly get to saved screen-shots.
  NOM_ASSERT( nom::init( argc, argv ) == true );

  atexit( nom::quit );

  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}

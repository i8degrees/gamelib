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
#include <map>

#include <SDL.h>

#include "gtest/gtest.h"

#include <Rocket/Core.h>
#include <Rocket/Core/Input.h>

// nom::VisualUnitTest framework
#include "nomlib/tests/VisualUnitTest.hpp"

#include <nomlib/config.hpp>
#include <nomlib/core/SDL_assertion_helpers.hpp>
#include <nomlib/system.hpp>
#include <nomlib/serializers.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>

#include "nomlib/tests/gui/datagrid/CardsPageDataSource.hpp"

using namespace Rocket::Core;

namespace nom {

/// \brief Event listener callback for "keydown" events generated by libRocket.
///
/// \param ev     The object containing event info.
/// \param store  The UIDataViewList widget to use.
/// \param db     The cards database to use for comparison against model
///               (table) data.
/// \param model  The data source model to use.
/// \param player_hand  The mock player hand object to use for holding card
///                     data, such as totals on number of cards of a type.
void on_keydown(  Rocket::Core::Event& ev,
                  UIDataViewList* store,
                  const std::shared_ptr<CardCollection> db,
                  std::shared_ptr<CardsPageDataSource> model,
                  std::map<int,Card>& player_hand )
{
  EXPECT_TRUE( store != nullptr );
  EXPECT_TRUE( store->document() != nullptr );
  EXPECT_TRUE( db != nullptr );
  EXPECT_TRUE( model != nullptr );

  int pg = model->page();
  int num_pages = model->total_pages();

  if( ev == "keydown" )
  {
    Input::KeyIdentifier key = NOM_SCAST(Input::KeyIdentifier, ev.GetParameter<int>("key_identifier", 0) );

    // Handle paging (left && right)
    if( key == Input::KI_LEFT && pg > 0 )
    {
      --pg;
    }
    else if( key == Input::KI_RIGHT && ( pg < num_pages - 1 ) )
    {
      ++pg;
    }

    // Handle cursor (up && down)
    // else if( key == Input::KI_UP && selected_card > 0 )
    // {
    //   --selected_card;
    // }
    // else if( key == Input::KI_DOWN && ( selected_card < per_page - 1 ) )
    // {
    //   ++selected_card;
    // }

    model->set_page(pg);
    // model->set_selection(selected_card);

    // Update left page indicator
    Element* page_left = store->document()->GetElementById("page-left");
    // NOM_ASSERT( page_left != nullptr );
    if( page_left ) {
      page_left->SetClassNames("");

      if( model->page() > 0 ) {
        if( page_left ) {
          page_left->SetClassNames("page-left");
        }
      }
    } // end if page_left

    // Update right page indicator
    Element* page_right = store->document()->GetElementById("page-right");
    // NOM_ASSERT( page_right != nullptr );
    if( page_right ) {
      page_right->SetClassNames("");

      if( model->page() >= 0 && (model->page() < model->total_pages() - 1) ) {
        if( page_right ) {
          page_right->SetClassNames("page-right");
        }
      }
    } // end if page_right

    // NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "KEYDOWN event from", target->GetId().CString(), "value:", key );
    NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "Pg:", model->page() );
    // NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "Selection:", model->selection() );
  } // end if keydown

  EXPECT_TRUE( store->set_column_title(1, "CARDS P. " + std::to_string(pg+1) ) );

  EXPECT_TRUE( model->per_page() == model->GetNumRows("cards") );
  EXPECT_EQ( 57, model->num_rows() );
  EXPECT_EQ( pg, model->page() );

} // end func on_keydown

/// \brief Event listener callback for "mouseup" events generated by libRocket.
///
/// \param ev     The object containing event info.
/// \param store  The UIDataViewList widget to use.
/// \param db     The cards database to use for comparison against model
///               (table) data.
/// \param model  The data source model to use.
/// \param player_hand  The mock player hand object to use for holding card
///                     data, such as totals on number of cards of a type.
void on_mouseup(  Rocket::Core::Event& ev,
                  UIDataViewList* store,
                  const std::shared_ptr<CardCollection> db,
                  std::shared_ptr<CardsPageDataSource> model,
                  std::map<int,Card>& player_hand )
{
  EXPECT_TRUE( store != nullptr );
  EXPECT_TRUE( db != nullptr );
  EXPECT_TRUE( model != nullptr );

  // ID of card selection
  int selection = 0;
  Rocket::Core::Element* target = ev.GetTargetElement();

  if( ev == "mouseup" )
  {
    Rocket::Core::Input::KeyIdentifier button =
      (Rocket::Core::Input::KeyIdentifier) ev.GetParameter<int>("button", 3);

    EXPECT_TRUE( model->per_page() == model->GetNumRows("cards") );
    EXPECT_EQ( 57, model->num_rows() );

    if( target ) {

      Card card = model->lookup_by_name( target->GetInnerRML().CString() );
      selection = card.id();

      if( target->GetTagName() == "card" && button == 0 ) // Left click
      {
        // Card selection logic; player hand receives a card
        //
        // 1. Decrease available card count by one
        // 2. Sync the cards model to reflect modified card count (-1)
        // 3. Update player hand w/ a copy of the reference card
        if( card.num() > 0 )
        {
          card.set_num( card.num() - 1 );
          model->insert_card(selection, card);
          player_hand[card.id()] = card;
        }

      } // end if button == 0
      else if( target->GetTagName() == "card" && button == 1 )  // Right click
      {
        // Compare the selected card from the current model with the game
        // database; we rely on the game database to be the "safe" -- read-only.
        Card ref_card = db->lookup_by_id(selection);

        // Card selection logic; player hand removes a card
        //
        // 1. Increase available card count by one
        // 2. Sync the cards model to reflect modified card count (+1)
        // 3. Remove the card from the player's hand
        if( card.num() < ref_card.num() )
        {
          card.set_num( card.num() + 1 );
          model->insert_card(selection, card);
          player_hand.erase( card.id() );
        }
      } // end if button == 1

      NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "Card ID:", selection );
      NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "Card name:", target->GetInnerRML().CString() );

    } // end if target

  } // end if click
} // end func on_mouseup

class libRocketDataGridTest: public nom::VisualUnitTest
{
  public:
    /// \remarks This method is called at the start of each unit test.
    libRocketDataGridTest() :
      VisualUnitTest( Size2i(768,448) )
    {
      // NOM_LOG_TRACE( NOM );

      // The frame image to compare against the reference image set
      this->append_screenshot_frame( 0 );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~libRocketDataGridTest()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks Initialization callback for VisualUnitTest to act on, instead
    /// of its default rendering setup. A bit of additional setup is
    /// required for plumbing in libRocket into our setup.
    bool init_rendering()
    {
      int render_driver = -1;
      uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
      uint32 render_flags = SDL_RENDERER_ACCELERATED;

      if( nom::set_hint( SDL_HINT_RENDER_VSYNC, "0" ) == false )
      {
        NOM_LOG_INFO ( NOM, "Could not disable vertical refresh." );
      }

      if( nom::set_hint( SDL_HINT_RENDER_SCALE_QUALITY, "nearest" ) == false )
      {
        NOM_LOG_INFO( NOM, "Could not set scale quality to", "nearest" );
      }

      // Required as per our libRocket implementation
      render_driver = nom::available_render_driver("opengl");
      if( render_driver == -1 )
      {
        NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION,
                      "Could not find an OpenGL rendering driver." );
        return false;
      }

      if( this->window_.create( this->test_case(),
                                this->resolution(),
                                window_flags,
                                render_driver,
                                render_flags ) == false )
      {
        NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Could not initialize rendering context and window." );
        return false;
      }

      if( nom::RocketSDL2RenderInterface::gl_init( this->window_.size().w, this->window_.size().h ) == false )
      {
        NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Could not initialize OpenGL for libRocket." );
        return false;
      }

      // Allow for automatic rescaling of the output window based on aspect
      // ratio (i.e.: handle fullscreen resizing); this will use letterboxing
      // when the aspect ratio is greater than what is available, or side-bars
      // when the aspect ratio is less than.
      this->render_window().set_logical_size( this->resolution() / Size2i(2,2) );
      // this->render_window().set_logical_size( this->resolution() );

      // Use pixel unit scaling; this gives us a one to two pixel ratio --
      // scale output display by a factor of two.
      this->render_window().set_scale( Point2f(2,2) );
      // this->render_window().set_scale( Point2f(1,1) );

      return true;
    }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp()
    {
      nom::SearchPath resources;
      std::string ext = ".json";

      // NOM_LOG_TRACE( NOM );

      // VisualUnitTest environment init
      VisualUnitTest::SetUp();

      // nom::init sets the working directory to this executable's directory
      // path; i.e.: build/tests or build/tests/Debug depending on build
      // environment. Everything is relative from here on out.

      // Determine our resources path based on several possible locations --
      // also dependent upon the build environment
      if( resources.load_file( nom::UnitTest::test_set() + ext, "resources" ) == false )
      {
        FAIL()
        << "Could not determine the resource path for "
        << nom::UnitTest::test_set() + ext;
      }

      // Initialize the core of libRocket; these are the core dependencies that
      // libRocket depends on for successful initialization.
      Rocket::Core::FileInterface* fs =
        new nom::RocketFileInterface( resources.path() );

      Rocket::Core::SystemInterface* sys =
        new RocketSDL2SystemInterface();

      if( nom::init_librocket( fs, sys ) == false )
      {
        FAIL()
        << "Could not initialize libRocket.";
      }

      Rocket::Core::RenderInterface* renderer =
        new nom::RocketSDL2RenderInterface( &this->window_ );

      // Initialize libRocket's debugger as early as possible, so we get visual
      // logging
      this->desktop.enable_debugger();
      if( this->desktop.create_context( "default", this->resolution(), renderer ) == false )
      {
        FAIL()
        << "Could not initialize libRocket context.";
      }

      // Maximum debugger height shall be no more than half the size of the
      // context, add menu height of debugger's button bar
      Size2i debugger_dims( 150, (this->desktop.size().h / 2) + 32 );
      this->desktop.set_debugger_size(debugger_dims);

      Size2i output_size( this->resolution().w / this->window_.scale().x,
                          this->resolution().h / this->window_.scale().y );

      EXPECT_EQ( output_size, this->desktop.size() );

      if( this->desktop.load_font( "Delicious-Bold.otf" ) == false )
      {
        FAIL() << "Could not load font file: Delicious-Bold.otf";
      }

      if( this->desktop.load_font( "OpenSans-Regular.ttf" ) == false )
      {
        FAIL() << "Could not load font file: OpenSans-Regular.ttf";
      }

      if( this->desktop.load_font( "OpenSans-Bold.ttf" ) == false )
      {
        FAIL() << "Could not load font file: OpenSans-Bold.ttf";
      }

      // Load custom decorators for nomlib

      Rocket::Core::DecoratorInstancer* decorator0 = new nom::DecoratorInstancerFinalFantasyFrame();
      Rocket::Core::Factory::RegisterDecoratorInstancer("final-fantasy-theme", decorator0 );
      decorator0->RemoveReference();

      DecoratorInstancerSprite* decorator1 = new nom::DecoratorInstancerSprite();
      Rocket::Core::Factory::RegisterDecoratorInstancer("sprite-sheet", decorator1 );
      decorator1->RemoveReference();

      /// Put our event polling within the main event's loop
      this->append_event_callback( [&] ( const Event ev ) { this->desktop.process_event( ev ); } );

      // Register GUI updates onto our main loop (::on_run).
      this->append_update_callback( [&] ( float delta ) { this->desktop.update(); } );

      // Register GUI rendering onto our main loop (::on_run).
      this->append_render_callback( [&] ( const RenderWindow& win ) { this->desktop.draw(); } );
    }

    /// \remarks This method is called before destruction, at the end of each
    /// unit test.
    virtual void TearDown()
    {
      // NOM_LOG_TRACE( NOM );

      this->desktop.shutdown();

      nom::shutdown_librocket();
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
    nom::UIContext desktop;

    /// \brief The back-end model used for the datagrid.
    std::shared_ptr<CardsPageDataSource> model;

    /// \brief Mock data source back-end
    std::shared_ptr<CardCollection> db;

    /// \brief Styling for the first column
    CardStatusFormatter card_status_formatter;

    /// \brief Styling for the second column
    CardNameFormatter card_name_formatter;

    /// \brief The mock player's hand of cards; mapping for selected cards
    std::map<int,Card> phand;
};

/// \todo Split up into smaller testing units
TEST_F( libRocketDataGridTest, DataSourceModel )
{
  // Disable assertion handling prompts for these tests (just report instead)
  // nom::set_assertion_handler(log_assert_handler, nullptr);
  // Disable assertion handling prompts for these tests and ignore the report
  // nom::set_assertion_handler(null_assert_handler, nullptr);

  // Buffer objects used for verifying storage model results
  nom::StringList row;
  nom::StringList cols;
  cols.push_back( "name" );
  cols.push_back( "num" );

  this->model.reset( new CardsPageDataSource("cards_db") );

  // Baseline sanity tests

  EXPECT_TRUE( this->model != nullptr );

  EXPECT_EQ( "cards", this->model->table_name() )
  << "Table name string should be the default initialized value.";

  EXPECT_EQ( true, this->model->empty() )
  << "Resulting storage size should be empty (zero).";

  // NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, this->model->dump() );

  // Creation tests

  CardList cards;
  cards.push_back( Card( 0, "Geezard", 999 ) );
  EXPECT_EQ( 1, this->model->append_cards( cards ) )
  << "Resulting storage size should be one item.";

  EXPECT_EQ( 2, this->model->append_card( Card( 89, "testme", 22 ) ) )
  << "Resulting storage size should be two items.";

  // Overwrite 'testme' card
  EXPECT_EQ( 2, this->model->insert_card( 1, Card( 1, "Fungar", 777 ) ) )
  << "Resulting storage size should remain two.";

  EXPECT_EQ( false, this->model->empty() )
  << "Storage should not be empty.";

  // NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, this->model->dump() );

  // Destruction tests

  // Destroy 'Fungar' card
  EXPECT_EQ( 1, this->model->erase_card( 1 ) )
  << "Resulting storage size should be one.";

  // ...and verify the results
  row.clear();
  this->model->row(row, 0, cols );
  EXPECT_EQ( "Geezard", row.at(0) );
  EXPECT_EQ( "999", row.at(1) );

  cards.clear();
  cards.push_back( Card( 2, "Bite Bug", 2 ) );
  cards.push_back( Card( 3, "Red Bat", 5 ) );
  cards.push_back( Card( 89, "Diablos", 666 ) );

  // Overwrite all but the first card in the model, shifting said first card to
  // the end of the container.
  EXPECT_EQ( 4, this->model->insert_cards( 0, cards ) )
  << "Resulting storage size should be four items.";

  // Ensure that the first card is at the end of the container
  row.clear();
  this->model->row( row, this->model->num_rows() - 1, cols );
  EXPECT_EQ( "Geezard", row.at(0) );
  EXPECT_EQ( "999", row.at(1) );
  // NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, this->model->dump() );

  // Destroy all but 'Geezard' card
  EXPECT_EQ( 1, this->model->erase_cards( 0, 3 ) )
  << "The Geezard card should be the only item remaining.";

  // ...and verify the results
  row.clear();
  this->model->row(row, 0, cols );
  EXPECT_EQ( "Geezard", row.at(0) );
  EXPECT_EQ( "999", row.at(1) );

  this->model->erase_cards();
  EXPECT_EQ( true, this->model->empty() )
  << "Resulting storage size should be emptied (zero).";

  row.clear();
  this->model->erase_cards();

  NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, this->model->dump() );
}

TEST_F( libRocketDataGridTest, UIDataViewList )
{
  Point2i pos1(60,25);
  std::string doc_file1 = "dataview.rml";

  UIDataViewList store1;
  EXPECT_EQ( true, store1.set_context(&this->desktop) );
  EXPECT_EQ( true, store1.load_document_file( doc_file1 ) )
  << this->test_set() << " object should not be invalid; is the context and document file valid?";

  // Ensure that the visual debugger's beacon (err icon) appears on-screen.
  Rocket::Core::Log::Message( Rocket::Core::Log::LT_ASSERT, "Hello, world!" );

  this->model.reset( new CardsPageDataSource("cards_db") );
  this->db.reset( new CardCollection() );
  EXPECT_TRUE( model != nullptr );
  EXPECT_TRUE( db != nullptr );

  EXPECT_EQ( true, db->load_db() )
  << "Could not initialize nom::CardsPageDataSource data interface.";

  CardList deck;
  CardList cards = db->cards();

  // Load in the entire cards database
  for( auto itr = cards.begin(); itr != cards.end(); ++itr )
  {
    deck.push_back( *itr );
  }

  // Deck of cards for the data source
  model->append_cards( deck );

  store1.show();

  EXPECT_TRUE( store1.set_column_title(1, "CARDS P. " + std::to_string(model->page() + 1) ) );

  EXPECT_EQ( true, store1.visible() );
  EXPECT_EQ( pos1, store1.position() );

  // Default values sanity

  EXPECT_EQ( "cards", model->table_name() );

  EXPECT_EQ( 11, model->per_page() );
  EXPECT_EQ( 0, model->page() );

  EXPECT_EQ( 10, model->map_page_row( 10, 0 ) )
  << "Selection should be between 0..11";

  EXPECT_EQ( 2, model->map_page_row( 13, 1 ) )
  << "Selection should be between 0..11";

  EXPECT_EQ( 10, model->map_page_row( 21, 1 ) )
  << "Selection should be between 0..11";

  EXPECT_EQ( "CARDS P. 1", store1.column_title(1) );
  EXPECT_EQ( "NUM.", store1.column_title(2) );

  store1.register_event_listener( store1.document(), "keydown", new nom::UIEventListener( [&]
    ( Rocket::Core::Event& ev ) { on_keydown( ev, &store1, db, model, this->phand ); }
  ));

  store1.register_event_listener( store1.document(), "mouseup", new nom::UIEventListener( [&]
    ( Rocket::Core::Event& ev ) { on_mouseup( ev, &store1, db, model, this->phand ); }
  ));

  // Synthesized user input events; we hope to capture these before the end of
  // the first frame
  Rocket::Core::Element* target = nullptr;
  Rocket::Core::Dictionary lclick;
  Rocket::Core::Dictionary rclick;
  lclick.Set("button","0"); // Left click
  rclick.Set("button","1"); // Left click

  // We must update the context before our cards model is filled
  this->desktop.update();

  target = store1.document()->GetElementById("Geezard");
  if( target ) {
    target->DispatchEvent("mouseup", lclick);
    // Should have zero cards remaining
  }

  target = store1.document()->GetElementById("Red Bat");
  if( target ) {
    target->DispatchEvent("mouseup", lclick);
    target->DispatchEvent("mouseup", lclick);
    target->DispatchEvent("mouseup", lclick);
    // Should have one cards remaining
  }

  target = store1.document()->GetElementById("Red Bat");
  if( target ) {
    target->DispatchEvent("mouseup", rclick);
    // Should have two cards remaining
  }

  target = store1.document()->GetElementById("Cockatrice");
  if( target ) {
    target->DispatchEvent("mouseup", lclick);
    target->DispatchEvent("mouseup", lclick);
    target->DispatchEvent("mouseup", lclick);
    // Should have zero cards remaining
  }

  // NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, store1.dump() );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // FIXME: This must be done before calling nom::init because of a
  // dependency on SDL's video subsystem being initialized first.
  // nom::init_third_party should be made explicit (by the end-user).
  if( nom::set_hint( SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES, "0" ) == false )
  {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                  "Could not disable Spaces support." );
  }

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

  // Log all messages from GUI
  // nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE );
  // nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  // No logging output from testing functions (i.e.: event info)
  nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_TEST, nom::NOM_LOG_PRIORITY_WARN );

  return RUN_ALL_TESTS();
}

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
#include <nomlib/config.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>

using namespace nom;

const nom::Path p;

/// \brief File path name of the resources directory; this must be a relative file path.
const std::string APP_RESOURCES_DIR = "Resources" + p.native();

/// \brief Name of our application.
const std::string APP_NAME = "nomlib's GUI | ListBox Widgets";

/// \brief Width, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_WIDTH = 800;

/// \brief Height, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_HEIGHT = 600;

const std::string RESOURCE_ICON = APP_RESOURCES_DIR + "icon.png";

/// \brief Relative filename path to saved screen shots.
///
/// Default path should resolve to the same directory as the output binary.
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

class App: public nom::SDLApp
{
  public:
    App( nom::int32 argc, char* argv[] ) :
      listbox_ex0{ nullptr },
      listbox_ex1{ nullptr },
      listbox_ex2{ nullptr }
    {
      NOM_LOG_TRACE ( NOM );

      // Fatal error; if we are not able to complete this step, it means that
      // we probably cannot rely on our resource paths!
      if ( nom::init ( argc, argv ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not initialize nomlib." );
        exit ( NOM_EXIT_FAILURE );
      }

      atexit(nom::quit);
    }

    ~App( void )
    {
      NOM_LOG_TRACE ( NOM );

      delete this->gui_window[0];
      delete this->gui_window[1];

      this->gui_window[0] = nullptr;
      this->gui_window[1] = nullptr;
    }

    bool on_init( void )
    {
      nom::uint32 window_flags = SDL_WINDOW_SHOWN;

      if ( nom::set_hint ( SDL_HINT_RENDER_VSYNC, "0" ) == false )
      {
        NOM_LOG_INFO ( NOM, "Could not disable vertical refresh." );
      }

      if ( this->window.create( APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags ) == false )
      {
        return false;
      }

      if ( this->window.set_window_icon ( RESOURCE_ICON ) == false )
      {
        nom::DialogMessageBox ( APP_NAME, "Could not load window icon: " + RESOURCE_ICON );
        return false;
      }

      this->window_size = this->window.size();

      // Scale window contents up by the new width & height
      this->window.set_logical_size ( this->window_size.x, this->window_size.y );

      // Top-level (root) window initialization:

      // Top-level window (relative to global "screen" coordinates)
      this->gui_window_pos[0] = nom::Point2i( 2, 2 );
      this->gui_window_size[0] = nom::Size2i( WINDOW_WIDTH - 4, WINDOW_HEIGHT - 4 );

      this->gui_window[0] = new nom::UIWidget( nullptr, -1, this->gui_window_pos[0], this->gui_window_size[0] );
      this->gui_window[0]->set_name( "Root" );
      this->gui_window[0]->set_title( this->gui_window[0]->name() );

      // Draw a frame so that we can visually see the maximal bounds of the
      // top-level window
      this->gui_window[0]->set_decorator( new nom::MinimalDecorator() );

      // Window-scope mouse button click events
      // FIXME: Temporarily disabled (to cease debugging output):
      // this->gui_window[0]->register_event_listener( nom::UIEvent::WINDOW_MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->window_on_click( ev ); } ) );

      // Layout container initialization:

      this->gui_window_pos[1] = Point2i( 25, 25 );
      this->gui_window_size[1] = Size2i( WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 );

      this->gui_window[1] = new nom::UIWidget( nullptr, -1, this->gui_window_pos[1], this->gui_window_size[1] );
      this->gui_window[1]->set_name( "Layout" );
      this->gui_window[1]->set_title( this->gui_window[1]->name() );

      // Draw a frame so that we can visually see the maximal bounds of the
      // layout
      this->gui_window[1]->set_decorator( new nom::MinimalDecorator() );

      nom::ItemStrings labels_ex0;
      labels_ex0.push_back( "boobies!" );
      labels_ex0.push_back( "more boobies!" );

      nom::ItemStrings labels_ex1;
      labels_ex1.push_back( "item_0" );
      labels_ex1.push_back( "item_1" );
      labels_ex1.push_back( "item_2" );

      nom::ItemStrings labels_ex2;
      labels_ex2.push_back( "item_0" );
      labels_ex2.push_back( "item_1" );
      labels_ex2.push_back( "item_2" );
      labels_ex2.push_back( "item_3" );

      // Our widgets to be used in the layout.
      this->listbox_ex0 = this->create_listbox( this->gui_window[1], Point2i::null, Size2i(50,24), "listbox_ex0", labels_ex0 );
      this->listbox_ex0->set_font( SystemFonts::cache().load_resource("VIII") );
      this->listbox_ex0->set_decorator( new nom::MinimalDecorator() );
      this->listbox_ex0->set_selected_text_color( nom::Color4i::Gray );
  // this->listbox_ex0->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->on_click( ev ); } ) );
  // this->listbox_ex0->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->on_click( ev ); } ) );

      this->listbox_ex1 = this->create_listbox( this->gui_window[1], Point2i::null, Size2i(50,24), "listbox_ex1", labels_ex1, 1 );
      this->listbox_ex1->set_decorator( new nom::MinimalDecorator() );
  // this->listbox_ex1->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->on_click( ev ); } ) );
  // this->listbox_ex1->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->on_click( ev ); } ) );

      this->listbox_ex2 = this->create_listbox( this->gui_window[1], Point2i::null, Size2i(50,24), "listbox_ex2", labels_ex2, 2 );
      this->listbox_ex2->set_decorator( new nom::MinimalDecorator() );
  // this->listbox_ex2->register_event_listener( nom::UIEvent::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->on_click( ev ); } ) );
  // this->listbox_ex2->register_event_listener( nom::UIEvent::MOUSE_UP, nom::UIEventCallback( [&] ( nom::UIWidgetEvent& ev ) { this->on_click( ev ); } ) );

      // nom::UIStyle::shared_ptr custom_style = nullptr;
      // custom_style.reset( new nom::UIStyle() );

      // custom_style->set_font_color( nom::Color4i::Black );
      // custom_style->set_font_size( 13 );
      // custom_style->set_font_style( nom::Text::Style::Bold );

// this->gui_window[0]->insert_child( this->create_listbox( this->gui_window[0], Point2i(102,400), Size2i(50,48), "expliclt_listbox_button", labels_ex2, 3 ) );

      this->gui_window[1]->insert_child( this->listbox_ex0 );
      this->gui_window[1]->insert_child( this->listbox_ex1 );
      this->gui_window[1]->insert_child( this->listbox_ex2 );

      // Listbox layout

      // FIXME: Widgets must not be added to the layout until *after* a) font
      // initialization (widget's size hint); b) item store initialization
      // (internal update is necessary within ListBox::set_item_store).
      this->listbox_layout = new nom::UIVBoxLayout( this->gui_window[1] );

      // TODO: Add unit tests for each variation
      if( this->listbox_ex0 != nullptr )
      {
        // FIXME:
        // this->listbox_ex0->set_size_policy( nom::UILayoutPolicy::Policy::Minimum, nom::UILayoutPolicy::Policy::Minimum );
      }

      UILayoutItem* item = nullptr;

      // TODO: Add unit tests for this
      // this->listbox_layout->set_spacing( 1 );

      this->listbox_layout->append_spacer( 8 );
      this->listbox_layout->append_widget( this->listbox_ex0 );
      this->listbox_layout->append_spacer( 8 );
      this->listbox_layout->append_widget( this->listbox_ex1 );
      this->listbox_layout->append_spacer( 40 );
      this->listbox_layout->append_widget( this->listbox_ex2 );

      // FIXME:
      // this->listbox_layout->set_alignment( this->listbox_ex1, nom::Anchor::Center );
      // this->listbox_layout->set_alignment( this->listbox_ex2, nom::Anchor::Center );

      // Relative to parent widget
      this->listbox_layout->set_position( nom::Point2i( 12, 25 ) );

      // Test0:
      // TODO: Add unit tests for this
      // this->button_layout->set_position( nom::Point2i( 0, 0 ) );

      item = this->listbox_layout->at( 0 );

      NOM_ASSERT( item->spacer_item() != nullptr );

      if( item->spacer_item() != nullptr )
      {
        NOM_ASSERT( item->spacer_item()->bounds().x == -1 );
        NOM_ASSERT( item->spacer_item()->bounds().y == -1 );

        // Should be the size as calculated by the layout (dependent upon size
        // policy).
        NOM_ASSERT( item->spacer_item()->size().w == 8 );
        NOM_ASSERT( item->spacer_item()->size().h == 8 );

        // NOM_DUMP( item->spacer_item()->bounds().x );
        // NOM_DUMP( item->spacer_item()->bounds().y );
        // NOM_DUMP( item->spacer_item()->size().w );
        // NOM_DUMP( item->spacer_item()->size().h );
      }

      // listbox_ex0
      item = this->listbox_layout->at( 1 );

      NOM_ASSERT( item->widget() != nullptr );

      if( item->widget() != nullptr )
      {
        // Absolute (global screen) coordinates
        // Should include both UISpacerItem spacing, but not internal layout
        // spacing (because it is the first item).
        NOM_ASSERT( item->widget()->position().x == 37 );
        NOM_ASSERT( item->widget()->position().y == 58 );

        // Should be the size as calculated by the layout (dependent upon size
        // policy).
        NOM_ASSERT( item->widget()->size().w == 50 );

        // FIXME:
        #if defined( NOM_PLATFORM_WINDOWS )
          NOM_ASSERT( item->widget()->size().h == 56 );
        #else
          NOM_ASSERT( item->widget()->size().h == 60 );
        #endif

        // NOM_DUMP( item->widget()->position().x );
        // NOM_DUMP( item->widget()->position().y );
        // NOM_DUMP( item->widget()->size().w );
        // NOM_DUMP( "listbox_ex0->size.h: ", item->widget()->size().h );
      }

      item = this->listbox_layout->at( 2 );

      NOM_ASSERT( item->spacer_item() != nullptr );

      if( item->spacer_item() != nullptr )
      {
        NOM_ASSERT( item->spacer_item()->bounds().x == -1 );
        NOM_ASSERT( item->spacer_item()->bounds().y == -1 );

        // Should be the size as calculated by the layout (dependent upon size
        // policy).
        NOM_ASSERT( item->spacer_item()->size().w == 8 );
        NOM_ASSERT( item->spacer_item()->size().h == 8 );

        // NOM_DUMP( item->spacer_item()->bounds().x );
        // NOM_DUMP( item->spacer_item()->bounds().y );
        // NOM_DUMP( item->spacer_item()->size().w );
        // NOM_DUMP( item->spacer_item()->size().h );
      }

      // listbox_ex1
      item = this->listbox_layout->at( 3 );

      NOM_ASSERT( item->widget() != nullptr );

      if( item->widget() != nullptr )
      {
        // Absolute (global screen) coordinates
        // Should include both UISpacerItem spacing & internal layout spacing
        NOM_ASSERT( item->widget()->position().x == 37 );

        // FIXME:
        #if defined( NOM_PLATFORM_WINDOWS )
          NOM_ASSERT( item->widget()->position().y == 123 );
        #else
          NOM_ASSERT( item->widget()->position().y == 127 );
        #endif

        // Should be the size as calculated by the layout (dependent upon size
        // policy).
        NOM_ASSERT( item->widget()->size().w == 50 );

        // FIXME:
        #if defined( NOM_PLATFORM_WINDOWS )
          NOM_ASSERT( item->widget()->size().h == 56 );
        #else
          NOM_ASSERT( item->widget()->size().h == 60 );
        #endif

        // NOM_DUMP( item->widget()->position().x );
        // NOM_DUMP( "listbox_ex1->pos.y: ", item->widget()->position().y );
        // NOM_DUMP( item->widget()->size().w );
        // NOM_DUMP( "listbox_ex1->size.h: ", item->widget()->size().h );
      }

      // UISpacerItem
      item = this->listbox_layout->at( 4 );

      NOM_ASSERT( item->spacer_item() != nullptr );

      if( item->spacer_item() != nullptr )
      {
        NOM_ASSERT( item->spacer_item()->bounds().x == -1 );
        NOM_ASSERT( item->spacer_item()->bounds().y == -1 );

        // Should be the size as calculated by the layout (dependent upon size
        // policy).
        NOM_ASSERT( item->spacer_item()->size().w == 40 );
        NOM_ASSERT( item->spacer_item()->size().h == 40 );

        // NOM_DUMP( item->spacer_item()->bounds().x );
        // NOM_DUMP( item->spacer_item()->bounds().y );
        // NOM_DUMP( item->spacer_item()->size().w );
        // NOM_DUMP( item->spacer_item()->size().h );
      }

      // listbox_ex2
      item = this->listbox_layout->at( 5 );

      NOM_ASSERT( item->widget() != nullptr );

      if( item->widget() != nullptr )
      {
        // Absolute (global screen) coordinates
        // Should include both UISpacerItem spacing & internal layout spacing
        NOM_ASSERT( item->widget()->position().x == 37 );

        // FIXME:
        #if defined( NOM_PLATFORM_WINDOWS )
          NOM_ASSERT( item->widget()->position().y == 220 );
        #else
          NOM_ASSERT( item->widget()->position().y == 228 );
        #endif

        // Should be the size as calculated by the layout (dependent upon size
        // policy).
        NOM_ASSERT( item->widget()->size().w == 50 );

        // FIXME:
        #if defined( NOM_PLATFORM_WINDOWS )
          NOM_ASSERT( item->widget()->size().h == 56 );
        #else
          NOM_ASSERT( item->widget()->size().h == 60 );
        #endif

        // NOM_DUMP( item->widget()->position().x );
        // NOM_DUMP( "listbox_ex2->pos.y: ", item->widget()->position().y );
        // NOM_DUMP( item->widget()->size().w );
        // NOM_DUMP( "listbox_ex2->size.h: ", item->widget()->size().h );
      }

      // Sanity check
      NOM_ASSERT( this->gui_window[1]->layout() == this->listbox_layout );

      // These coordinates should be relative to the parent window that the
      // layout is attached to.
      NOM_ASSERT( this->listbox_layout->bounds().x == 12 );
      NOM_ASSERT( this->listbox_layout->bounds().y == 25 );

      // TODO: Unit test for button_layout size dimensions

      // FIXME:
      // NOM_DUMP( this->button_layout->bounds() );

      NOM_ASSERT( this->gui_window[1]->position().x == 25 );
      NOM_ASSERT( this->gui_window[1]->position().y == 25 );

      // Auto-generated name
      if( this->listbox_ex2 != nullptr )
      {
        NOM_ASSERT( this->gui_window[1]->find_child( "listbox_ex2" ) != nullptr );
      }

      NOM_ASSERT( this->gui_window[0]->id() == 1 );
      NOM_ASSERT( this->gui_window[0]->name() == "Root" );
      NOM_ASSERT( this->gui_window[0]->title() == "Root" );

      NOM_ASSERT( this->gui_window[1]->id() == 2 );

      NOM_ASSERT( this->gui_window[1]->name() == "Layout" );
      NOM_ASSERT( this->gui_window[1]->title() == "Layout" );

      nom::UIWidget* widget = nullptr;

      // button_ex0
      widget = this->gui_window[1]->find_child( 6 );

      if( widget != nullptr )
      {
        NOM_ASSERT( widget->is_window() == false );
        NOM_ASSERT( widget->name() == "listbox_ex0" );
      }

      NOM_ASSERT( this->gui_window[0]->is_window() == true );
      NOM_ASSERT( this->gui_window[1]->is_window() == true );

      return true;
    }

    /// \brief Test the creation of a button widget.
    nom::ListBox::raw_ptr create_listbox( const UIWidget::raw_ptr window, const Point2i& pos, const Size2i& size, const std::string& name, const nom::ItemStrings& labels, int sel = -1 )
    {
      nom::UIItemContainer::raw_ptr storage = new nom::UIItemContainer();
      storage->append( labels );

      if( sel != -1 )
      {
        storage->set_selection( sel );
      }

      nom::ListBox::raw_ptr listbox = new nom::ListBox( window, -1, pos, size, storage );

      listbox->set_name( name );
      NOM_ASSERT( listbox->name() == name );

      NOM_ASSERT( listbox->parent()->position() == window->position() );
      NOM_ASSERT( listbox->size() == size );

      return listbox;
    }

    void on_click( const nom::UIWidgetEvent& ev ) const
    {
      NOM_DUMP( ev.id() );
      NOM_DUMP( ev.index() );
      NOM_DUMP( ev.text() );
    }

    nom::int32 Run( void )
    {
      this->update.start();
      this->fps.start();

      // 1. Events
      // 2. Logic
      // 3. Render
      while ( this->running() == true )
      {
        while( this->poll_event( this->event ) )
        {
          this->on_event( this->event );

          this->gui_window[0]->process_event( this->event );
          this->gui_window[1]->process_event( this->event );
        }

        this->window.update();
        this->fps.update();

        // Refresh the frames per second at 1 second intervals
        if ( this->update.ticks() > 1000 )
        {
          if ( this->show_fps() == true )
          {
            this->window.set_window_title ( APP_NAME + " - " + this->fps.asString() + ' ' + "fps" );
          }
          else
          {
            this->window.set_window_title ( APP_NAME + " [" + std::to_string(this->window.window_id()) + "]" + " - " + "Display" + ' ' + std::to_string ( this->window.window_display_id() ) );
          }

          this->update.restart();
        } // end refresh cycle

        this->gui_window[0]->update();
        this->gui_window[1]->update();

        this->window.fill( nom::Color4i::SkyBlue );

        this->gui_window[0]->draw( this->window );
        this->gui_window[1]->draw( this->window );

      } // while app_state is true (running)

      return NOM_EXIT_SUCCESS;
    }

  private:
    /// \brief Event handler for key down actions.
    ///
    /// Implements the nom::Input::on_key_down method.
    void on_key_down( const nom::Event& ev )
    {
      switch ( ev.key.sym )
      {
        default: break;

        // Use inherited SDLApp::on_app_quit() method -- you may also provide
        // your own event handler for this.
        case SDLK_ESCAPE:
        case SDLK_q: this->on_app_quit( ev ); break;

        case SDLK_F1:
        {
          if( this->window.save_screenshot( OUTPUT_SCREENSHOT_FILENAME ) == false )
          {
            nom::DialogMessageBox( APP_NAME, "Could not save screenshot");
          } // end save_screenshot err check

          break;
        }

        case SDLK_f:
        {
          this->window.toggle_fullscreen();
          break;
        } // end SDLK_f

      } // end switch key
    } // on_key_down

  private:
    nom::Event event;

    /// Window handles
    nom::RenderWindow window;

    nom::Point2i window_size;

    /// Interval at which we refresh the frames per second counter
    nom::Timer update;

    /// Timer for tracking frames per second
    nom::FPS fps;

    nom::Point2i gui_window_pos[2];
    nom::Size2i gui_window_size[2];
    nom::UIWidget::raw_ptr gui_window[2];

    nom::ListBox* listbox_ex0;
    nom::ListBox* listbox_ex1;
    nom::ListBox* listbox_ex2;

    nom::UIBoxLayout* listbox_layout;
};

nom::int32 main( nom::int32 argc, char* argv[] )
{
  App app( argc, argv );

  if( app.on_init() == false )
  {
    nom::DialogMessageBox( APP_NAME, "Could not initialize application." );
    return NOM_EXIT_FAILURE;
  }

  return app.Run();

  // ...Goodbye cruel world..!
}

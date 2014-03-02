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

// Pubic nomlib interface headers
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>
// #include <nomlib/gui.hpp>

/// File path name of the resources directory; this must be a relative file path.
const std::string APP_RESOURCES_DIR = "Resources";

/// \brief  Relative file path name of our resource example
const nom::Path p;
const std::string RESOURCE_ICON = APP_RESOURCES_DIR + p.native() + "icon.png";

/// \brief Relative filename path to saved screen shot example
///
/// Default path should resolve to the same directory as the app example
/// executable
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

/// \brief Name of our application.
const std::string APP_NAME = "Examples Template App";

/// \brief Width, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_WIDTH = 768;

/// \brief Height, in pixels, of our effective rendering surface.
const nom::int32 WINDOW_HEIGHT = 448;

/// \brief Maximum number of active windows we will attempt to spawn in this
/// example.
const nom::int32 MAXIMUM_WINDOWS = 2;

const nom::int32 USER_EVENT_DEBUG = 0;

/// \brief Usage example
/// \remarks For unit testing: ensure that library is compiled with
/// the appropriate defines enabled within EventHandler.hpp.
class App: public nom::SDLApp
{
  public:
    App( nom::int32 argc, char* argv[] )
    {
      NOM_LOG_TRACE( NOM );

      // Fatal error; if we are not able to complete this step, it means that
      // we probably cannot rely on our resource paths!
      if( nom::init( argc, argv ) == false )
      {
        nom::DialogMessageBox( APP_NAME, "ERROR: Could not initialize nomlib." );
        exit( NOM_EXIT_FAILURE );
      }

      atexit( nom::quit );
    } // end App

    ~App( void )
    {
      NOM_LOG_TRACE( NOM );
    } // end ~App

    bool on_init( void )
    {
      nom::uint32 window_flags = SDL_WINDOW_RESIZABLE;

      for( auto idx = 0; idx < MAXIMUM_WINDOWS; ++idx )
      {
        if ( this->window[idx].create( APP_NAME, ( WINDOW_WIDTH / 2 ), WINDOW_HEIGHT, window_flags ) == false )
        {
          return false;
        }

        this->window[idx].set_position( 0 + ( WINDOW_WIDTH / 2 ) * idx, ( WINDOW_HEIGHT / 2 ) );

        if( this->window[idx].set_window_icon( RESOURCE_ICON ) == false )
        {
          nom::DialogMessageBox( APP_NAME, "ERROR: Could not load window icon: " + RESOURCE_ICON );
          return false;
        }

        this->window_size[idx] = this->window[idx].size();

        // Scale window contents up by the new width & height
        // this->window[idx].set_logical_size( this->window_size[idx].x, this->window_size[idx].y );
      }

      // Start out execution with both windows minimized.
      nom::EventDispatcher dispatcher;
      nom::EventCallback delegate1( [&] { this->minimize(0); } );
      nom::EventCallback delegate2( [&] { this->minimize(1); } );

      dispatcher.dispatch( nom::UserEvent( USER_EVENT_DEBUG, nullptr, NOM_SCAST( nom::EventCallback*, &delegate1 ), 0 ) );
      dispatcher.dispatch( nom::UserEvent( USER_EVENT_DEBUG, nullptr, NOM_SCAST( nom::EventCallback*, &delegate2 ), 0 ) );

      // C++ style syntax for adding an input map:
      this->add_input_mapping ( "minimize_window_0",
                                nom::Action (
                                              SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT, nom::EventCallback( [&] { this->minimize(0); } )
                                            )
                              );

      nom::EventCallback restore_callback( [&] { this->restore(0); } );

      // C style syntax for adding an input map:
      nom::Action restore_window_0;
      restore_window_0.type = SDL_MOUSEBUTTONDOWN;
      restore_window_0.event = SDL_BUTTON_RIGHT;
      restore_window_0.callback = restore_callback;
      this->add_input_mapping( "restore_window_0", restore_window_0 );

      return true;
    } // end on_init

    void minimize( int idx )
    {
      this->window[idx].minimize_window();
    }

    void restore( int idx )
    {
      this->window[idx].restore_window();
    }

    nom::sint Run( void )
    {
      for( auto idx = 0; idx < MAXIMUM_WINDOWS; ++idx )
      {
        this->update[idx].start();
        this->fps[idx].start();
      }

      // 1. Events
      // 2. Logic
      // 3. Render

      while( this->running() == true )
      {
        while( this->poll_event( &this->event ) )
        {
          this->on_event( &this->event );
        }

        for( auto idx = 0; idx < MAXIMUM_WINDOWS; ++idx )
        {
          this->window[idx].update();
          this->fps[idx].update();

          // Refresh the frames per second at 1 second intervals
          if ( this->update[idx].ticks() > 1000 )
          {
            if ( this->show_fps() == true )
            {
              this->window[idx].set_window_title( APP_NAME + " - " + this->fps[idx].asString() + ' ' + "fps" );
            }
            else
            {
              this->window[idx].set_window_title( APP_NAME + " [" + std::to_string(this->window[idx].window_id()) + "]" + " - " + "Display" + ' ' + std::to_string ( this->window[idx].window_display_id() ) );
            }

            this->update[idx].restart();
          } // end refresh cycle
        } // end for MAXIMUM_WINDOWS update loop

        for( auto idx = 0; idx < MAXIMUM_WINDOWS; ++idx )
        {
          this->window[idx].fill( nom::Color4i::SkyBlue );
        }
      } // end while SDLApp::running() is true

      return NOM_EXIT_SUCCESS;
    } // end Run()

  private:
    /// \brief Event handler for user-defined events.
    ///
    /// \remarks Implements nom::EventHandler::on_user_event
    void on_user_event( const nom::UserEvent& ev )
    {
      // A call is made here to the virtual method being re-implemented here in
      // order to catch debugging output with debug builds compiled in; see
      // EventHandler.hpp.
      // Input::on_user_event( ev );

      if( ev.code == USER_EVENT_DEBUG )
      {
        nom::EventCallback* delegate = ev.get_callback();

        if( delegate != nullptr )
        {
          delegate->execute();
        }
      }
    }

  private:
    /// \brief Window handles
    ///
    /// \TODO Use std::vector?
    nom::RenderWindow window[MAXIMUM_WINDOWS];

    nom::Point2i window_size[MAXIMUM_WINDOWS];

    /// \brief Interval at which we refresh the frames per second counter
    nom::Timer update[MAXIMUM_WINDOWS];

    /// \brief Timer for tracking frames per second
    nom::FPS fps[MAXIMUM_WINDOWS];
}; // end class App

nom::sint main( nom::int32 argc, char* argv[] )
{
  App app ( argc, argv );

  if ( app.on_init() == false )
  {
    nom::DialogMessageBox( APP_NAME, "ERROR: Could not initialize application." );
    return NOM_EXIT_FAILURE;
  }

  return app.Run();

  // ...Goodbye cruel world!
}

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
#ifndef NOMLIB_TESTS_COMMON_VISUAL_UNIT_TEST_HPP
#define NOMLIB_TESTS_COMMON_VISUAL_UNIT_TEST_HPP

#include <iostream>
#include <string>
#include <functional>
#include <vector>

#include "gtest/gtest.h"

#include "nomlib/config.hpp"
#include "nomlib/tests/common/UnitTest.hpp"
#include "nomlib/tests/common/ImageTestSet.hpp"
#include "nomlib/tests/common/ImageDiff.hpp"
#include "nomlib/graphics/RenderWindow.hpp"
#include "nomlib/system/Timer.hpp"
#include "nomlib/system/FPS.hpp"
#include "nomlib/system/Event.hpp"
#include "nomlib/system/EventHandler.hpp"
#include "nomlib/system/InputMapper/InputStateMapper.hpp"

namespace nom {

/// \brief Base class interface for visual unit testing within Google Test
class VisualUnitTest: public UnitTest
{
  public:
    /// \remarks Access to global VisualUnitTest::results_ (test result totals)
    /// and VisualUnitTest::timestamp_ (file & directory sync).
    friend class VisualUnitTestResultWriter;

    typedef std::function<void( Event )> event_callback_type;
    typedef std::function<void( float )> update_callback_type;
    typedef std::function<void( const RenderWindow& )> render_callback_type;

    /// \brief Initialize the environment for the unit test.
    ///
    /// \remarks The necessary directory tree -- if any (this is dependent upon
    /// parsed command line options) -- is initialized at this time. The
    /// recorded timestamp of the visual unit test will be the set upon the
    /// first constructed object instance of this class.
    void initialize( const Size2i& res );

    /// \brief Default constructor.
    ///
    /// \remarks The resolution default of 640 x 480 is used.
    VisualUnitTest( void );

    /// \brief Construct the object using a non-default rendering resolution.
    ///
    /// \remarks The default resolution set for the rendering window is
    /// initialized to 640 x 480 -- to change the rendering resolution, a call
    /// to ::set_resolution must be made before ::SetUp is called.
    ///
    /// If using the image comparison features of the class, you must append
    /// at least one screen-shot frame before the ::on_run loop is executed.
    VisualUnitTest( const Size2i& res );

    /// \brief Destructor.
    ///
    /// \remarks Destruction of the object flushes the configuration file for
    /// the visual test set when it has images in its list.
    virtual ~VisualUnitTest( void );

    /// \brief Initialize the rendering window and setup the default input
    /// bindings used for each unit test.
    ///
    /// \remarks Unless you are re-implementing the initialization for the
    /// rendering window and input mappings, a call to this base method should
    /// be made.
    ///
    /// The intended resolution should be set -- using ::set_resolution --
    /// before this method is called.
    ///
    /// \note Re-implements ::testing::SetUp.
    ///
    /// \see nom::RenderWindow, nom::InputStateMapper, ::testing::SetUp.
    virtual void SetUp( void );

    /// \brief Compare the current test run image against a reference set.
    ///
    /// \remarks This method supports Google Test's EXPECT & friends macros;
    /// typical usage is EXPECT_TRUE( this->compare() ).
    ///
    /// \todo Allow margin of err in image diff tests?
    /// See also:
    /// [Allow approximate comparison with a margin of error #38](https://github.com/facebook/ios-snapshot-test-case/issues/38)
    /// [AllimageCompare](https://github.com/aleph7/AIImageCompare)
    ::testing::AssertionResult compare( void );

    /// \brief Get the visibility state of the FPS counter.
    bool fps( void ) const;

    /// \brief The resolution (width and height, in pixels) used for creating
    /// a rendering window for the test.
    const Size2i& resolution( void ) const;

    /// \brief The output directory prefix used for the unit test.
    const std::string& output_directory( void ) const;

    /// \brief Get the directory path prefix used for the current test case.
    const std::string& test_set_directory( void ) const;

    /// \brief Get the directory path used for dumping screen-shots and its
    /// associated meta-data.
    const std::string& test_reference_directory( void ) const;

    /// \brief Get the filename used for outputting screen-shots.
    const std::string& output_filename( void ) const;

    /// \brief Get the rendering window used for this test.
    ///
    /// \remarks A validity check (assertion) is done for the rendering window.
    ///
    /// \see ::SetUp.
    RenderWindow& render_window( void );

    /// \brief Toggle the visibility state of the FPS counter.
    void show_fps( bool state );

    /// \brief Set the render window's resolution.
    void set_resolution( const Size2i& res );

    /// \brief Set the output directory prefix used for the unit test.
    void set_output_directory( const std::string& dir_path );

    /// \brief Add a frame number to the list of frames to screen capture (for
    /// use as either a reference or comparison image set).
    ///
    /// \remarks If using the image comparison features of the class, you must
    /// append at least one screen-shot frame before the ::on_run loop is
    /// executed.
    void append_screenshot_frame( uint frame );

    virtual int on_run( void );

    /// \fixme Screen-shot dumps of more than the first frame in the list are
    /// broken.
    virtual bool on_frame_end( uint elapsed_frames );

    // void save_screenshot( const std::string& file_path );

    int append_event_callback( const std::function<void( Event )>& func );
    int append_update_callback( const std::function<void( float )>& func );
    int append_render_callback( const std::function<void( const RenderWindow& )>& func );

  protected:
    /// \brief Track visibility state of the FPS counter.
    bool show_fps_;

    /// \brief Interval at which we refresh the FPS counter
    Timer fps_counter_update_;

    /// \brief FPS counter
    FPS fps_counter_;

    /// \brief The rendering window (including its rendering context) used.
    RenderWindow window_;

    /// \brief The polling method used for input events.
    EventHandler evt_;

    /// \brief The registered input bindings.
    InputStateMapper input_mapper_;

    static ImageTestSet visual_test_;

  private:
    /// \brief Create the necessary directory tree for screen-shot dumps.
    ///
    /// \remarks This modifies the output directory.
    ///
    /// \see ::set_output_directory.
    ///
    /// \todo Clean up the logic in this method.
    void initialize_directories( void );
    // void initialize_directories( const std::string& dir_name );

    /// \brief Get the timestamp string recorded for the visual unit test.
    static const std::string& timestamp( void );

    /// \brief Set the directory prefix used for the current unit test case.
    void set_test_set_directory( const std::string& dir_path );

    /// \brief Set the directory prefix for dumping unit test reference
    /// screen-shots.
    void set_test_reference_directory( const std::string& dir_path );

    void set_output_filename( const std::string& filename );

    /// \brief The common timestamp string shared across object instances.
    static std::string timestamp_;

    /// \brief Ensure one-time only initialization of the timestamp string.
    ///
    /// \remarks This check prevents object instances from potentially creating
    /// multiple output directories for one set of screen-shot dumps. (Within
    /// the context of being ran within the GTest environment, multiple object
    /// instances within a short time are quite common).
    static bool timestamp_initialized_;

    /// \brief The resolution used for the creation of a rendering window.
    Size2i resolution_;

    std::vector<uint> screenshot_frames_;

    /// \remarks The output directory path in use.
    std::string output_directory_;

    /// \remarks The directory path prefix used for the current test case.
    std::string test_set_directory_;

    /// \remarks The directory path used for test reference screen-shots and
    /// meta-data file.
    std::string test_reference_directory_;

    std::string output_filename_;

    std::vector<event_callback_type> event_callbacks_;
    std::vector<update_callback_type> update_callbacks_;
    std::vector<render_callback_type> render_callbacks_;

    static ImageDiffResultBatch results_;
};

/// \brief Convenience macro for binding an event listener to the unit test's
/// main loop.
#define NOM_TEST_ADD_EVENT(type, func) \
  ( this->append_event_callback( [&] ( type ) { func; } ) )

/// \brief Convenience macro for binding an update method to the unit test's
/// main loop.
#define NOM_TEST_ADD_UPDATE(func) \
  ( this->append_update_callback( [&] ( float delta ) { func; } ) )

/// \brief Convenience macro for binding a rendering method to the unit test's
/// main loop.
#define NOM_TEST_ADD_RENDER(type, func ) \
  ( this->append_render_callback( [&] ( type ) { func; } ) )

} // namespace nom

#endif // include guard defined

/// \class nom::VisualUnitTest
/// \ingroup tests
///
/// \see Adapted from the implementation for [Ogre3D](http://www.ogre3d.org/tikiwiki/Visual+Unit+Testing+Framework) by Riley Adams <praetor57@gmail.com>.
///
/// \see https://github.com/facebook/ios-snapshot-test-case
///

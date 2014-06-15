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
#include "gui/common.hpp"

// Private headers
#include "gtest/gtest.h"

namespace nom {

namespace priv {

void on_click( const nom::UIWidgetEvent& ev )
{
  NOM_DUMP( ev.id() );
  NOM_DUMP( ev.index() );
  NOM_DUMP( ev.text() );
}

void expected_layout_spacer_pos( const UILayout* layout, int idx, const Point2i& pos )
{
  ASSERT_TRUE( layout != nullptr );

  UILayoutItem::raw_ptr item = layout->at( idx );

  ASSERT_TRUE( item != nullptr );
  ASSERT_TRUE( item->spacer_item() != nullptr );

  if( item->spacer_item() != nullptr )
  {
    // NOM_DUMP( item->spacer_item()->bounds().x );
    // NOM_DUMP( item->spacer_item()->bounds().y );

    EXPECT_EQ( pos.x, item->spacer_item()->bounds().x );
    EXPECT_EQ( pos.y, item->spacer_item()->bounds().y );
  } // end spacer item
}

void expected_layout_spacer_dims( const UILayout* layout, int idx, const Size2i& dims )
{
  ASSERT_TRUE( layout != nullptr );

  UILayoutItem::raw_ptr item = layout->at( idx );

  ASSERT_TRUE( item != nullptr );
  ASSERT_TRUE( item->spacer_item() != nullptr );

  if( item->spacer_item() != nullptr )
  {
    // NOM_DUMP( item->spacer_item()->size().w );
    // NOM_DUMP( item->spacer_item()->size().h );

    // Should be the size as calculated by the layout (dependent upon size
    // policy).
    EXPECT_EQ( dims.w, item->spacer_item()->size().w );
    EXPECT_EQ( dims.h, item->spacer_item()->size().h );
  } // end spacer item
}

void expected_layout_spacer_output( const UILayout* layout, int idx, const Point2i& pos, const Size2i& dims )
{
  priv::expected_layout_spacer_pos( layout, idx, pos );
  priv::expected_layout_spacer_dims( layout, idx, dims );
}

void expected_layout_widget_pos( const UILayout* layout, int idx, const Point2i& pos )
{
  ASSERT_TRUE( layout != nullptr );

  UILayoutItem::raw_ptr item = nullptr;

  item = layout->at( idx );

  ASSERT_TRUE( item != nullptr );
  ASSERT_TRUE( item->widget() != nullptr );

  if( item->widget() != nullptr )
  {
    // NOM_DUMP( item->widget()->name() );
    // NOM_DUMP( item->widget()->bounds().x );
    // NOM_DUMP( item->widget()->bounds().y );

    // Absolute (global screen) coordinates
    // Should include both UISpacerItem spacing, but not internal layout
    // spacing (because it is the first item).
    EXPECT_EQ( pos.x, item->widget()->position().x );
    EXPECT_EQ( pos.y, item->widget()->position().y );
  } // end widget item
}

void expected_layout_widget_dims( const UILayout* layout, int idx, const Size2i& dims )
{
  ASSERT_TRUE( layout != nullptr );

  UILayoutItem::raw_ptr item = nullptr;

  item = layout->at( idx );

  ASSERT_TRUE( item != nullptr );
  ASSERT_TRUE( item->widget() != nullptr );

  if( item->widget() != nullptr )
  {
    // NOM_DUMP( item->widget()->name() );
    // NOM_DUMP( item->widget()->size().w );
    // NOM_DUMP( item->widget()->size().h );

    // Should be the size as calculated by the layout (dependent upon size
    // policy).
    EXPECT_EQ( dims.w, item->widget()->size().w );
    EXPECT_EQ( dims.h, item->widget()->size().h );
  } // end widget item
}

void expected_layout_widget_output( const UILayout* layout, int idx, const Point2i& pos, const Size2i& dims )
{
  ASSERT_TRUE( layout != nullptr );

  UILayoutItem::raw_ptr item = nullptr;

  item = layout->at( idx );

  ASSERT_TRUE( item != nullptr );
  ASSERT_TRUE( item->widget() != nullptr );

  if( item->widget() != nullptr )
  {
    priv::expected_layout_widget_pos( layout, idx, pos );
    priv::expected_layout_widget_dims( layout, idx, dims );
  } // end widget item
}

void expected_layout_output( const UILayout* layout, const std::vector<Point2i>& pos, const std::vector<Size2i>& dims )
{
  // Element position within layout
  int idx = 0;

  // First widget item
  idx = 1;
  priv::expected_layout_widget_pos( layout, idx, pos.at(0) );
  priv::expected_layout_widget_dims( layout, idx, dims.at(0) );

  // Last widget item
  idx = layout->count() - 1;
  priv::expected_layout_widget_pos( layout, idx, pos.at(1) );
  priv::expected_layout_widget_dims( layout, idx, dims.at(1) );
}

UIBoxLayout* create_layout( UIWidget* window, const std::vector<UIWidget*>& items, const std::vector<int>& spacers, const std::string& name, Orientations dir, int spacing )
{
  NOM_ASSERT( window != nullptr );

  nom::UIBoxLayout* layout = nullptr;

  for( auto itr = items.begin(); itr != items.end(); ++itr )
  {
    window->insert_child( *itr );
  }

  if( dir == Orientations::Horizontal )
  {
    layout = new nom::UIHBoxLayout( window );
  }
  else // Orientations::Vertical
  {
    layout = new nom::UIVBoxLayout( window );
  }

  layout->set_spacing( spacing );
  EXPECT_EQ( spacing, layout->spacing() );

  int idx = 0;
  for( auto itr = items.begin(); itr != items.end(); ++itr )
  {
    if( idx < spacers.size() )
    {
      layout->append_spacer( spacers.at( idx ) );
    }

    layout->append_widget( *itr );

    ++idx;
  }

  IntRect layout_bounds( window->position(), window->size() );
  layout->set_bounds( layout_bounds );

  EXPECT_EQ( layout->bounds().x, window->position().x );
  EXPECT_EQ( layout->bounds().y, window->position().y );
  EXPECT_EQ( layout->bounds().w, window->size().w );
  EXPECT_EQ( layout->bounds().h, window->size().h );

  NOM_ASSERT( window->layout() == layout );

  return layout;
}

void set_layout_alignment( UILayout* layout, uint32 align )
{
  ASSERT_FALSE( layout == nullptr );

  layout->set_alignment( align );
  EXPECT_EQ( align, layout->alignment() );
}

void set_widget_alignment( UILayout* layout, const UIWidget* window, uint32 align )
{
  int pos = -1;
  UILayoutItem* item = nullptr;

  ASSERT_FALSE( layout == nullptr );

  EXPECT_EQ( true, layout->set_alignment( window, align ) );

  ASSERT_FALSE( window == nullptr );

  pos = layout->find_widget( window );

  EXPECT_NE( -1, pos );

  item = layout->at( pos );

  ASSERT_FALSE( item == nullptr );

  if( item != nullptr )
  {
    EXPECT_EQ( align, item->alignment() );
  }
}

} // namespace priv

} // namespace nom

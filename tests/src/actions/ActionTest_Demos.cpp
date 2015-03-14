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
#include "nomlib/tests/actions/ActionTest.hpp"

namespace nom {

struct test_action
{
  std::shared_ptr<Sprite> sprite;
  std::shared_ptr<MoveByAction> action;
  std::shared_ptr<RepeatForAction> container;
  Point2i sprite_pos = Point2i::null;
  real32 speed = 1.0f;
};

typedef std::map<uint32, test_action> test_action_list;

/// \brief This test is intended to simulate a worst-case scenario, i.e.: a
/// large number of objects enqueued and deallocated at the same time.
///
/// \note When building this test under Windows, iterator debugging can slow
/// this test down massively! See also: _ITERATOR_DEBUG_LEVEL=0
TEST_F(ActionTest, RainingRectsStressTest)
{
  // Testing parameters
  const real32 DURATION = 2.0f;
  IActionObject::timing_curve_func timing_mode = nullptr;
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const real32 MIN_SPEED_MOD = DURATION;
  const real32 MAX_SPEED_MOD = MIN_SPEED_MOD + NOM_ACTION_TEST_FLAG(speed);
  const nom::size_type NUM_REPEATS = 4;
  const nom::size_type NUM_OBJECTS = NOM_ACTION_TEST_FLAG(num_objects);

  const Size2i MIN_RECT_SIZE(8, 8);
  const Size2i MAX_RECT_SIZE(16, 16);

  // Position delta applied over duration
  const Point2i TRANSLATE_POS( Point2i(0, WINDOW_DIMS.h-MAX_RECT_SIZE.h) );

  if( NOM_ACTION_TEST_FLAG(timing_mode_str) != "linear_ease_in_out" ) {
    timing_mode = NOM_ACTION_TEST_FLAG(timing_curve);
  } else {
    // Default used timing curve when the end-user does not override us
    timing_mode = nom::Bounce::ease_in_out;
  }

  if( NOM_ACTION_TEST_FLAG(enable_vsync) == false ) {
    NOM_LOG_WARN( NOM_LOG_CATEGORY_APPLICATION,
                  "Passing --enable-vsync may help boost test performance",
                  "considerably!" );
  }

  auto rand_seed = nom::ticks();
  nom::init_rand(rand_seed);
  NOM_LOG_DEBUG(NOM_LOG_CATEGORY_ACTION, "Random seed value:", rand_seed);

  test_action_list actions;

  // drawables generation
  int x_offset = 0;
  for( auto idx = 0; idx != NUM_OBJECTS; ++idx ) {

    Point2i pos(x_offset, 0);
    Size2i dims(Size2i::zero);
    Color4i color;

    int16 red = nom::uniform_real_rand<real32>(0.0f,255.0f);
    int16 green = nom::uniform_real_rand<real32>(0.0f,255.0f);
    int16 blue = nom::uniform_real_rand<real32>(0.0f,255.0f);
    color = Color4i(red, green, blue);

    dims.w =
      nom::uniform_real_rand<real32>(MIN_RECT_SIZE.w, MAX_RECT_SIZE.w);
    dims.h =
      nom::uniform_real_rand<real32>(MIN_RECT_SIZE.h, MAX_RECT_SIZE.h);

    if( x_offset >= 640 ) {
      x_offset = 0;
    } else {
      x_offset += dims.w + (dims.w / 2);
    }

    IntRect rect_bounds(pos, dims);

    auto rect =
      new Rectangle(rect_bounds, color);
    ASSERT_TRUE(rect != nullptr);

    auto sprite = std::make_shared<Sprite>( rect->texture() );
    ASSERT_TRUE(sprite != nullptr);
    NOM_DELETE_PTR(rect);

    actions[idx].sprite_pos = sprite->position();
    actions[idx].sprite = sprite;
  }

  for( auto idx = 0; idx != NUM_OBJECTS; ++idx ) {
    auto translate =
      nom::create_action<MoveByAction>(actions[idx].sprite, TRANSLATE_POS, DURATION);
    ASSERT_TRUE(translate != nullptr);
    translate->set_name("MoveByAction" + std::to_string(idx) );
    actions[idx].action = translate;

    real32 random_speed_mod =
      nom::uniform_real_rand<real32>(MIN_SPEED_MOD, MAX_SPEED_MOD);
    translate->set_speed(random_speed_mod);
    actions[idx].speed = random_speed_mod;

    auto repeat =
      nom::create_action<RepeatForAction>(translate, NUM_REPEATS);
    ASSERT_TRUE(repeat != nullptr);
    repeat->set_name("RepeatForAction_MoveByAction" + std::to_string(idx) );
    actions[idx].container = repeat;
  }

  action_list group_actions;
  for( auto idx = 0; idx != NUM_OBJECTS; ++idx ) {
    group_actions.emplace_back(actions[idx].container);
  }

  auto action0 =
    nom::create_action<GroupAction>(group_actions);
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(timing_mode);
  action0->set_name("group_action0");

  auto remove_action0 =
    nom::create_action<RemoveAction>(action0);
  ASSERT_TRUE(remove_action0 != nullptr);
  remove_action0->set_name("remove_action0");

  this->run_action_ret =
  this->player.run_action(action0, [=]() mutable {

    for( auto idx = 0; idx != NUM_OBJECTS; ++idx ) {

      auto sprite = actions[idx].sprite.get();
      auto sprite_pos = actions[idx].sprite_pos;
      ASSERT_TRUE(sprite != nullptr);
      EXPECT_EQ(sprite_pos.x, sprite->position().x);
      EXPECT_EQ(TRANSLATE_POS.y, sprite->position().y);

      auto action = actions[idx].action.get();
      ASSERT_TRUE(action != nullptr);
      this->expected_common_params( action, DURATION, actions[idx].speed,
                                    "action_speed" );

      auto action_container = actions[idx].container.get();
      ASSERT_TRUE(action_container != nullptr);
      this->expected_repeat_params( action_container, NUM_REPEATS,
                                    "action_num_repeats" );
    }

    this->player.run_action(remove_action0, [=]() mutable {

      for( auto idx = 0; idx != NUM_OBJECTS; ++idx ) {

        auto sprite = actions[idx].sprite.get();
        ASSERT_TRUE(sprite != nullptr);
        EXPECT_FALSE( sprite->valid() );
      }
    });

  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";

  this->append_update_callback( [=](real32) {
    if( this->expected_min_duration(DURATION, MAX_SPEED_MOD) == true ) {
      this->quit();
    }
  });

  for( auto idx = 0; idx != NUM_OBJECTS; ++idx ) {
    auto drawable = actions[idx].sprite.get();
    this->append_render_queue(drawable);
  }

  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, CardPlacementEffectsDemo)
{
  // Testing parameters
  const float DURATION = 0.5f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const Size2f SCALE_FACTOR(3.0f, 3.0f);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(128, 128);

  const Point2i MOVE_UP0_TRANSLATE_POS(192, -320);
  const Point2i MOVE_UP1_TRANSLATE_POS(-320+128, -192-128);

  // Expected total change over time
  const Point2i A1_EXPECTED_TEX_POS(192,32);
  const Size2i A1_EXPECTED_TEX_SIZE(  TEX_SIZE.w * SCALE_FACTOR.w,
                                      TEX_SIZE.h * SCALE_FACTOR.h);
  const Point2i A2_EXPECTED_TEX_POS(192,32);
  const Point2i A3_EXPECTED_TEX_POS(320,32);

  // The texture will be twice its size when the second action to reverse the
  // rescaling starts running
  const Size2i A2_EXPECTED_TEX_SIZE(TEX_SIZE.w, TEX_SIZE.h);
  const Size2i A3_EXPECTED_TEX_SIZE(TEX_SIZE.w, TEX_SIZE.h);

  auto bg_tex = std::make_shared<Texture>();
  ASSERT_TRUE(bg_tex != nullptr);
  if( bg_tex->load( resources[0].path() + "backdrop.png" ) == false ) {
    FAIL()  << "Could not load 'backdrop.png' input file from "
            << resources[0].path();
  }
  if( bg_tex->valid() == false ) {
    FAIL() << "Texture is not valid.";
  }

  // Stretched dimensions to cover entire window
  bg_tex->set_size(WINDOW_DIMS);

  // action0, action1
  auto tex0 = std::make_shared<Texture>();
  ASSERT_TRUE(tex0 != nullptr);
  tex0->load(resources[0].path() + "card.png");
  if( tex0->valid() == false ) {
    FAIL() << "Could not load texture from file source.";
  }

  // action2, action3
  auto tex1 = std::make_shared<Texture>();
  ASSERT_TRUE(tex1 != nullptr);
  tex1->load(resources[0].path() + "card.png");
  if( tex1->valid() == false ) {
    FAIL() << "Could not load texture from file source.";
  }

  auto sprite0 = std::make_shared<Sprite>();
  ASSERT_TRUE(sprite0 != nullptr);
  sprite0->set_texture(tex0);
  sprite0->set_size(TEX_SIZE);

  auto sprite1 = std::make_shared<Sprite>();
  ASSERT_TRUE(sprite1 != nullptr);
  sprite1->set_texture(tex1);
  sprite1->set_size(TEX_SIZE);

  nom::set_alignment(sprite0.get(), Point2i::zero, WINDOW_DIMS, Anchor::BottomLeft);
  ASSERT_TRUE( sprite0->position() == Point2i(0, 352) )
  << sprite0->position();

  nom::set_alignment(sprite1.get(), Point2i::zero, WINDOW_DIMS, Anchor::BottomRight);
  ASSERT_TRUE( sprite1->position() == Point2i(512, 352) )
  << sprite1->position();

  auto move_up0 =
    nom::create_action<MoveByAction>(sprite0, MOVE_UP0_TRANSLATE_POS, DURATION);
  ASSERT_TRUE(move_up0 != nullptr);

  auto move_up1 =
    nom::create_action<MoveByAction>(sprite1, MOVE_UP1_TRANSLATE_POS, DURATION);
  ASSERT_TRUE(move_up1 != nullptr);

  auto scale_by0 =
    nom::create_action<nom::ScaleByAction>(sprite0, SCALE_FACTOR, DURATION);
  ASSERT_TRUE(scale_by0 != nullptr);

  auto scale_by1 =
    nom::create_action<nom::ScaleByAction>(sprite1, SCALE_FACTOR, DURATION);
  ASSERT_TRUE(scale_by1 != nullptr);

  auto scale_by_reversed0 =
    nom::create_action<nom::ReversedAction>( scale_by0->clone() );
  ASSERT_TRUE(scale_by_reversed0 != nullptr);

  auto scale_by_reversed1 =
    nom::create_action<nom::ReversedAction>( scale_by1->clone() );
  ASSERT_TRUE(scale_by_reversed1 != nullptr);

  move_up0->set_name("move_up0");
  move_up1->set_name("move_up1");

  scale_by0->set_name("scale_by0");
  scale_by1->set_name("scale_by1");

  scale_by_reversed0->set_name("scale_by_reversed0");
  scale_by_reversed1->set_name("scale_by_reversed1");

  auto action0 =
    nom::create_action<GroupAction>( {move_up0, scale_by0} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(nom::Expo::ease_out);
  action0->set_speed(SPEED_MOD);
  action0->set_name("action0");

  auto action1 =
    nom::create_action<GroupAction>( {scale_by_reversed0} );
  ASSERT_TRUE(action1 != nullptr);
  action1->set_timing_curve(nom::Expo::ease_out);
  action1->set_speed(SPEED_MOD);
  action1->set_name("action1");

  auto action2 =
    nom::create_action<GroupAction>( {move_up1, scale_by1} );
  ASSERT_TRUE(action2 != nullptr);
  action2->set_timing_curve(nom::Expo::ease_out);
  action2->set_speed(SPEED_MOD);
  action2->set_name("action2");

  auto action3 =
    nom::create_action<GroupAction>( {scale_by_reversed1} );
  ASSERT_TRUE(action3 != nullptr);
  action3->set_timing_curve(nom::Expo::ease_out);
  action3->set_speed(SPEED_MOD);
  action3->set_name("action3");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    // move_up0
    EXPECT_EQ( A1_EXPECTED_TEX_POS, sprite0->position() );
    // scale_by0
    EXPECT_EQ( A1_EXPECTED_TEX_SIZE, sprite0->size() );
    EXPECT_EQ(2, this->player.num_actions() );

    this->expected_action_params(action0.get(), 2);
    this->expected_common_params(move_up0.get(), DURATION, SPEED_MOD);
    this->expected_common_params(scale_by0.get(), DURATION, SPEED_MOD);

    this->player.run_action(action1, [=]() {

      // move_up0
      EXPECT_EQ( A2_EXPECTED_TEX_POS, sprite0->position() );
      // scale_by_reversed0
      EXPECT_EQ( A2_EXPECTED_TEX_SIZE, sprite0->size() );
      EXPECT_EQ(2, this->player.num_actions() );

      this->expected_action_params(action1.get(), 1);
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  EXPECT_EQ(1, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action2, [=]() {

    // move_up1
    EXPECT_EQ( A3_EXPECTED_TEX_POS, sprite1->position() );
    // scale_by1
    EXPECT_EQ( A1_EXPECTED_TEX_SIZE, sprite1->size() );
    EXPECT_EQ(3, this->player.num_actions() );

    this->expected_action_params(action2.get(), 2);
    this->expected_common_params(move_up1.get(), DURATION, SPEED_MOD);
    this->expected_common_params(scale_by1.get(), DURATION, SPEED_MOD);

    this->player.run_action(action3, [=]() {

      // move_up1
      EXPECT_EQ( A3_EXPECTED_TEX_POS, sprite1->position() );
      // scale_by_reversed1
      EXPECT_EQ( A3_EXPECTED_TEX_SIZE, sprite1->size() );
      EXPECT_EQ(2, this->player.num_actions() );

      this->expected_action_params(action3.get(), 1);
    });
  });

  this->append_update_callback( [=](float) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_queue( bg_tex.get() );
  this->append_render_queue( sprite0.get() );
  this->append_render_queue( sprite1.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, ScrollingTextDemo)
{
  nom::Font font;
  nom::Text rendered_text;

  // #define DEV_SCROLLING_TEXT_NO_WAIT_TIMER

  // Testing parameters
  const real32 DURATION = 15.5f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  const real32 MOVE_BY_DURATION = 2.5f;
  const real32 FADE_OUT_DURATION = 2.5f;
  const real32 FADE_IN_DURATION = 2.5f;
  const real32 NUM_REPEATS = 2;

#if defined(DEV_SCROLLING_TEXT_NO_WAIT_TIMER)
  const real32 WAIT1S_DURATION = 0.0f;
  const real32 WAIT2S_DURATION = 0.0f;
#else
  const real32 WAIT1S_DURATION = 1.0f;
  const real32 WAIT2S_DURATION = 2.0f;
#endif

  auto bg_tex = std::make_shared<Texture>();
  if( bg_tex->load( resources[0].path() + "backdrop.png" ) == false ) {
    NOM_LOG_WARN( NOM_LOG_CATEGORY_APPLICATION,
                  "Could not load 'backdrop.png' input file from ",
                  resources[0].path() );
  }
  if( (bg_tex != nullptr) && bg_tex->valid() == false ) {
    NOM_LOG_WARN( NOM_LOG_CATEGORY_APPLICATION,
                  "Texture for 'backdrop.png' is not valid!" );
  }

  // Stretched dimensions to cover entire window
  bg_tex->set_size(WINDOW_DIMS);

  auto bg_sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(bg_sprite != nullptr);
  EXPECT_EQ(true, bg_sprite->set_texture(bg_tex) );

  nom::set_file_root( resources[1].path() );
  if( font.load( resources[1].path() + "gameover.fnt" ) == false ) {
    FAIL()  << "Could not load 'gameover.fnt' input file from "
            << resources[1].path();
  }

  rendered_text.set_font(&font);
  rendered_text.set_text("Hello, world!");
  rendered_text.set_position( Point2i(0,0) );

  // This is a deep-copy clone of rendered_text object's text texture; used for
  // animation of the displacement
  auto anim_text_tex =
    std::shared_ptr<Texture>( rendered_text.clone_texture() );
  ASSERT_TRUE(anim_text_tex != nullptr);

  auto anim_text_sprite = std::make_shared<Sprite>();
  ASSERT_TRUE(anim_text_sprite != nullptr);
  EXPECT_EQ(true, anim_text_sprite->set_texture(anim_text_tex) );

  // This is sharing the same texture used for the animated text, in order to
  // modify the same texture data for alpha blending animations
  auto anim_fade_tex =
    std::shared_ptr<Texture>( anim_text_sprite->texture() );
  ASSERT_TRUE(anim_fade_tex != nullptr);

  auto anim_fade_sprite = std::make_shared<Sprite>();
  ASSERT_TRUE(anim_fade_sprite != nullptr);
  EXPECT_EQ(true, anim_fade_sprite->set_texture(anim_fade_tex) );

  nom::set_alignment( anim_text_sprite.get(), rendered_text.position(),
                      WINDOW_DIMS, Anchor::MiddleLeft );

  // Initialize texture with its starting alpha value for testing alpha
  // blending to full opacity
  anim_fade_sprite->set_alpha(Color4i::ALPHA_TRANSPARENT);

  auto anim_translate0 =
    nom::create_action<nom::MoveByAction>(anim_text_sprite, Point2i(200, 0), MOVE_BY_DURATION);

  auto anim_translate2 =
    nom::create_action<nom::MoveByAction>(anim_text_sprite, Point2i(0,300), MOVE_BY_DURATION);

  auto anim_fade_out0 =
    nom::create_action<nom::FadeOutAction>(anim_fade_sprite, FADE_OUT_DURATION);

  auto anim_fade_in =
    nom::create_action<nom::FadeInAction>(anim_fade_sprite, FADE_IN_DURATION);
  ASSERT_TRUE(anim_fade_in != nullptr);

  auto wait1s =
    nom::create_action<nom::WaitForDurationAction>(WAIT1S_DURATION);
  ASSERT_TRUE(wait1s != nullptr);

  auto wait2s =
    nom::create_action<nom::WaitForDurationAction>(WAIT2S_DURATION);
  ASSERT_TRUE(wait2s != nullptr);

  auto wait3s =
    nom::create_action<SequenceAction>( {wait1s, wait2s} );
  ASSERT_TRUE(wait3s != nullptr);
  wait3s->set_speed(SPEED_MOD);
  wait3s->set_timing_curve(TIMING_MODE);
  wait3s->set_name("wait3s");

  auto scroll_right =
    nom::create_action<GroupAction>( {anim_translate0, anim_fade_in} );
  ASSERT_TRUE(scroll_right != nullptr);
  scroll_right->set_speed(SPEED_MOD);
  scroll_right->set_timing_curve(TIMING_MODE);
  scroll_right->set_name("scroll_right");
  if( NOM_ACTION_TEST_FLAG(timing_mode_str) != "linear_ease_in_out" ) {
    scroll_right->set_timing_curve(TIMING_MODE);
  } else {
    scroll_right->set_timing_curve(nom::Quad::ease_in_out);
  }

  std::shared_ptr<IActionObject> scroll_left_action;
  scroll_left_action = anim_translate0->clone();

  auto scroll_left =
    nom::create_action<nom::ReversedAction>(scroll_left_action);
  ASSERT_TRUE(scroll_left != nullptr);
  scroll_left->set_speed(SPEED_MOD);
  scroll_left->set_name("scroll_left");
  if( NOM_ACTION_TEST_FLAG(timing_mode_str) != "linear_ease_in_out" ) {
    scroll_left->set_timing_curve(TIMING_MODE);
  } else {
    scroll_left->set_timing_curve(nom::Expo::ease_out);
  }

  auto scroll_right_again_action =
    nom::create_action<nom::MoveByAction>(anim_text_sprite, Point2i(200,0), MOVE_BY_DURATION);
  ASSERT_TRUE(scroll_right_again_action != nullptr);

  auto scroll_right_again =
    nom::create_action<GroupAction>( {scroll_right_again_action} );
  ASSERT_TRUE(scroll_right_again != nullptr);
  scroll_right_again->set_speed(SPEED_MOD);
  scroll_right_again->set_timing_curve(TIMING_MODE);
  scroll_right_again->set_name("scroll_right_again");

  auto scroll_up_move_action =
    nom::create_action<nom::MoveByAction>(anim_text_sprite, Point2i(0,-300), MOVE_BY_DURATION);
  ASSERT_TRUE(scroll_up_move_action != nullptr);

  auto scroll_up =
    nom::create_action<GroupAction>( {
      scroll_up_move_action, anim_fade_out0->clone() } );
  ASSERT_TRUE(scroll_up != nullptr);
  scroll_up->set_speed(SPEED_MOD);
  scroll_up->set_name("scroll_up");
  if( NOM_ACTION_TEST_FLAG(timing_mode_str) != "linear_ease_in_out" ) {
    scroll_up->set_timing_curve(TIMING_MODE);
  } else {
    scroll_up->set_timing_curve(nom::Quad::ease_in);
    scroll_up->set_speed(SPEED_MOD);
  }

  auto scroll_down_repeat2_translate =
    nom::create_action<nom::RepeatForAction>(anim_translate2, NUM_REPEATS);
  ASSERT_TRUE(scroll_down_repeat2_translate != nullptr);

  auto scroll_down_repeat2_fade_out =
    nom::create_action<nom::RepeatForAction>(anim_fade_out0, NUM_REPEATS);
  ASSERT_TRUE(scroll_down_repeat2_fade_out != nullptr);

  auto scroll_down_repeat2 =
    nom::create_action<GroupAction>( {
      scroll_down_repeat2_translate, scroll_down_repeat2_fade_out} );
  ASSERT_TRUE(scroll_down_repeat2 != nullptr);
  scroll_down_repeat2->set_speed(SPEED_MOD);
  scroll_down_repeat2->set_name("scroll_down_repeat2");
  if( NOM_ACTION_TEST_FLAG(timing_mode_str) != "linear_ease_in_out" ) {
    scroll_down_repeat2->set_timing_curve(TIMING_MODE);
  } else {
    scroll_down_repeat2->set_timing_curve(nom::Bounce::ease_out);
  }

  auto fade_screen_out =
    nom::create_action<nom::FadeOutAction>(bg_sprite, FADE_OUT_DURATION);

  auto fade_screen_out_action =
    nom::create_action<SequenceAction>( {fade_screen_out} );
  ASSERT_TRUE(fade_screen_out_action != nullptr);
  fade_screen_out_action->set_speed(SPEED_MOD);
  fade_screen_out_action->set_name("fade_screen_out");
  if( NOM_ACTION_TEST_FLAG(timing_mode_str) != "linear_ease_in_out" ) {
    fade_screen_out_action->set_timing_curve(TIMING_MODE);
  } else {
    fade_screen_out_action->set_timing_curve(nom::Bounce::ease_in_out);
  }

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(wait3s, [=]() {
    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_action_params(wait3s.get(), 2);

    this->expected_common_params( wait1s.get(), WAIT1S_DURATION, SPEED_MOD,
                                  "wait1s" );
    this->expected_common_params( wait2s.get(), WAIT2S_DURATION, SPEED_MOD,
                                  "wait2s" );
    this->player.run_action(scroll_right, [=]() {

      EXPECT_EQ(1, this->player.num_actions() );
      EXPECT_EQ( Point2i(200,195), anim_text_sprite->position() );

      this->expected_alpha_in_params( anim_fade_in.get(),
                                      Color4i::ALPHA_OPAQUE,
                                      anim_fade_sprite.get() );

      this->expected_action_params(scroll_right.get(), 2, "scroll_right" );

      this->expected_common_params( anim_translate0.get(), MOVE_BY_DURATION,
                                    SPEED_MOD, "scroll_right" );
      this->expected_common_params( anim_fade_in.get(), FADE_IN_DURATION,
                                    SPEED_MOD, "scroll_right" );

      this->player.run_action(scroll_left, [=]() {

        EXPECT_EQ(1, this->player.num_actions() );
        EXPECT_EQ( Point2i(0,195), anim_text_sprite->position() );
        EXPECT_EQ( Color4i::ALPHA_OPAQUE, anim_fade_sprite->alpha() );

        this->expected_common_params( scroll_left_action.get(),
                                      MOVE_BY_DURATION, SPEED_MOD,
                                      "scroll_left" );

        this->player.run_action(scroll_right_again, [=]() {

          EXPECT_EQ(1, this->player.num_actions() );
          EXPECT_EQ( Point2i(200,195), anim_text_sprite->position() );
          EXPECT_EQ( Color4i::ALPHA_OPAQUE, anim_fade_sprite->alpha() );

          this->expected_action_params( scroll_right_again.get(), 1,
                                        "scroll_right_again" );
          this->expected_common_params( scroll_right_again_action.get(),
                                        MOVE_BY_DURATION, SPEED_MOD,
                                        "scroll_right_again" );

          this->player.run_action(scroll_up, [=]() {

            EXPECT_EQ(1, this->player.num_actions() );
            EXPECT_EQ( Point2i(200,-105), anim_text_sprite->position() );
            this->expected_alpha_out_params(  anim_fade_out0.get(),
                                              Color4i::ALPHA_TRANSPARENT,
                                              nullptr,
                                              "scroll_up" );
            this->expected_action_params(scroll_up.get(), 2, "scroll_up" );
            this->expected_common_params( scroll_up_move_action.get(),
                                          MOVE_BY_DURATION, SPEED_MOD,
                                          "scroll_up" );
            this->expected_common_params( anim_fade_out0.get(),
                                          FADE_OUT_DURATION, SPEED_MOD,
                                          "scroll_up" );

            // Reset the initial alpha value for the next action to re-use;
            // we are using a shallow-copy of the fade out action -- the
            // underlying texture data is shared across clones
            anim_fade_sprite->set_alpha(Color4i::ALPHA_OPAQUE);

            this->player.run_action(scroll_down_repeat2, [=]() {

              EXPECT_EQ(1, this->player.num_actions() );
              EXPECT_EQ( Point2i(200,300), anim_text_sprite->position() );
              this->expected_alpha_out_params(  anim_fade_out0.get(),
                                                Color4i::ALPHA_TRANSPARENT,
                                                anim_fade_sprite.get(),
                                                "scroll_down_repeat2" );

              this->expected_repeat_params( scroll_down_repeat2_translate.get(),
                                            NUM_REPEATS,
                                            "scroll_down_repeat2" );
              this->expected_repeat_params( scroll_down_repeat2_fade_out.get(),
                                            NUM_REPEATS,
                                            "scroll_down_repeat2" );
              this->expected_action_params( scroll_down_repeat2.get(), 2,
                                            "scroll_down_repeat2" );

              this->expected_common_params( anim_translate2.get(),
                                            MOVE_BY_DURATION, SPEED_MOD,
                                            "scroll_down_repeat2" );
              this->expected_common_params( anim_fade_out0.get(),
                                            FADE_OUT_DURATION, SPEED_MOD,
                                            "scroll_down_repeat2" );

              this->player.run_action(fade_screen_out_action, [=]() {

                EXPECT_EQ(1, this->player.num_actions() );
                this->expected_alpha_out_params(  fade_screen_out.get(),
                                                  Color4i::ALPHA_TRANSPARENT,
                                                  bg_sprite.get(), "fade_screen_out" );

                this->expected_action_params( fade_screen_out_action.get(), 1,
                                              "fade_screen_out" );
              });
            });
          });
        });
      });
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue one or more actions!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // For the aesthetic appeal of fading the screen out
    this->render_window().fill(Color4i::Black);
  });

  this->append_render_queue( bg_sprite.get() );
  this->append_render_queue( anim_text_sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

} // namespace nom

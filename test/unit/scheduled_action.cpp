#include <rpg/operators/guid.hpp>
#include <rpg/scheduled_action.hpp>
#include <rpg/test/mocks/guid.hpp>

#include <SFML/System/Clock.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>

TEST(scheduled_action, scheduled_actions_have_unique_guid) {
  using rpg::operators::operator""_guid;
  rpg::test::mocks::guid guid{};
  EXPECT_CALL(guid, generate())
      .Times(1)
      .WillOnce(::testing::Return("0000000000000001"_guid));
  rpg::scheduled_action action{guid};
  EXPECT_EQ("0000000000000001"_guid, action.guid());
}

TEST(scheduled_action, do_thing_at_end_of_time) {
  using rpg::operators::operator""_guid;
  rpg::test::mocks::guid guid{};
  EXPECT_CALL(guid, generate())
      .Times(1)
      .WillOnce(::testing::Return("0000000000000001"_guid));
  auto x = 0;
  rpg::scheduled_action action(guid, std::chrono::seconds{1}, [&] { x = 42; });
  EXPECT_EQ(0, x);
  auto delta_time = sf::seconds(0.5f);
  action.update(delta_time);
  EXPECT_EQ(0, x);
  action.update(delta_time);
  EXPECT_EQ(42, x);
  x = 10;
  action.update(sf::seconds(2.0f));
  EXPECT_EQ(10, x);
}

TEST(scheduled_action, can_cancel_scheduled_action) {
  using rpg::operators::operator""_guid;
  rpg::test::mocks::guid guid{};
  EXPECT_CALL(guid, generate())
      .Times(1)
      .WillOnce(::testing::Return("0000000000000001"_guid));
  auto x = 0;
  rpg::scheduled_action action(guid, std::chrono::seconds{1}, [&] { x = 42; });
  EXPECT_EQ(0, x);
  auto delta_time = sf::seconds(0.5f);
  action.update(delta_time);
  EXPECT_EQ(0, x);
  action.cancel();
  EXPECT_EQ(0, x);
  action.update(delta_time);
  EXPECT_EQ(0, x);
}

TEST(scheduled_action, can_pause_scheduled_action) {
  using rpg::operators::operator""_guid;
  rpg::test::mocks::guid guid{};
  EXPECT_CALL(guid, generate())
      .Times(1)
      .WillOnce(::testing::Return("0000000000000001"_guid));
  auto x = 0;
  rpg::scheduled_action action(guid, std::chrono::seconds{1}, [&] { x = 42; });
  EXPECT_EQ(0, x);
  auto delta_time = sf::seconds(0.5f);
  action.update(delta_time);
  EXPECT_EQ(0, x);
  action.pause();
  EXPECT_EQ(0, x);
  action.update(delta_time);
  EXPECT_EQ(0, x);
}

TEST(scheduled_action, resume_scheduled_actions) {
  using rpg::operators::operator""_guid;
  rpg::test::mocks::guid guid{};
  EXPECT_CALL(guid, generate())
      .Times(1)
      .WillOnce(::testing::Return("0000000000000001"_guid));
  auto x = 0;
  rpg::scheduled_action action(guid, std::chrono::seconds{1}, [&] { x = 42; });
  EXPECT_EQ(0, x);
  auto delta_time = sf::seconds(0.5f);
  action.update(delta_time);
  EXPECT_EQ(0, x);
  action.pause();
  action.update(delta_time);
  EXPECT_EQ(0, x);
  action.resume();
  action.update(delta_time);
  EXPECT_EQ(42, x);
}

#if defined(RPG_OS_IS_WINDOWS)

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
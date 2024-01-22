#pragma once

#include <gmock/gmock.h>

namespace rpg::test::mocks {
struct speed {
  MOCK_METHOD(float, frontal_movement, (), (const noexcept));
  MOCK_METHOD(float, backward_movement, (), (const noexcept));
  MOCK_METHOD(float, lateral_movement, (), (const noexcept));
  MOCK_METHOD(float, rotational_movement, (), (const noexcept));
};
} // namespace rpg::test::mocks
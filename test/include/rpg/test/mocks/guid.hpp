#pragma once

#include <rpg/guid.hpp>

#include <gmock/gmock.h>

namespace rpg::test::mocks {
struct guid {
  MOCK_METHOD(rpg::guid, generate, ());
};
} // namespace rpg::test::mocks
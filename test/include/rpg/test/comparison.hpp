#pragma once

#include <gtest/gtest.h>

#include <cmath>
#include <concepts> //NOLINT
#include <limits>

namespace rpg::test::inline comparision {
template <std::floating_point T, std::floating_point U>
auto expect_equal(const T lhs, const U rhs) {
  if constexpr (sizeof(T) <= sizeof(U)) {
    const auto value = std::fabs(lhs - static_cast<T>(rhs));

    const auto epsilon =
        std::numeric_limits<T>::epsilon() * std::fabs(lhs) >= std::fabs(rhs)
            ? std::fabs(lhs)
            : std::fabs(rhs);
    EXPECT_TRUE(value <= epsilon)
        << std::setprecision(16) << "lhs: " << lhs << " rhs: " << rhs
        << " diff: " << value << " epsilon: " << epsilon;
  }
  const auto value = std::fabs(static_cast<U>(lhs) - rhs);
  const auto epsilon =
      std::numeric_limits<U>::epsilon() *
      (std::fabs(lhs) >= std::fabs(rhs) ? std::fabs(lhs) : std::fabs(rhs));
  EXPECT_TRUE(value <= epsilon)
      << std::setprecision(16) << "lhs: " << lhs << " rhs: " << rhs
      << " diff: " << value << " epsilon: " << epsilon;
}
} // namespace rpg::test::inline comparision
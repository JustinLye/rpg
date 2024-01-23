#pragma once

#include <rpg/guid.hpp>

#include <chrono>
#include <optional>

namespace rpg {
template <class T = decltype([] {})> class scheduled_action {
  rpg::guid guid_;
  float seconds_to_wait_;
  std::optional<T> action_;
  float seconds_elapsed_waiting_;
  bool paused_;

  auto should_update_() const noexcept {
    return action_.has_value() and not paused_;
  }

public:
  template <class TGuid>
  scheduled_action(TGuid &guid) : guid_(guid.generate()) {}

  template <class TGuid>
  scheduled_action(TGuid &guid, const auto time_to_wait, T &&action)
      : guid_(guid.generate()),
        seconds_to_wait_(
            std::chrono::duration_cast<std::chrono::seconds>(time_to_wait)
                .count()),
        action_(action) {}

  [[nodiscard]] auto guid() const noexcept { return guid_; }

  void cancel() {
    if (action_) {
      action_.reset();
    }
  }

  void pause() { paused_ = true; }

  void resume() { paused_ = false; }

  void update(const auto delta_time) {
    if (not should_update_()) {
      return;
    }

    seconds_elapsed_waiting_ += delta_time.asSeconds();
    if (seconds_elapsed_waiting_ < seconds_to_wait_) {
      return;
    }

    (*action_)();
    action_.reset();
  }
};

} // namespace rpg
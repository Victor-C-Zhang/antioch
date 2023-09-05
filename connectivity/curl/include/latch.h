// Copyright Antioch. All rights reserved.

#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>

namespace antioch {
namespace connectivity {
namespace curl_transfer {

class Latch {
 public:
  static constexpr ptrdiff_t max() noexcept { return 0x8FFF; }
  explicit Latch(ptrdiff_t expected) noexcept : mCount_(expected) {}
  ~Latch() = default;
  Latch(const Latch&) = delete;
  Latch& operator=(const Latch&) = delete;

  void count_down(ptrdiff_t update = 1);

  bool try_wait() const noexcept;

  void wait() const noexcept;

  void arrive_and_wait(ptrdiff_t update = 1) noexcept;

  template<class Rep, class Period>
  bool wait_or_timeout(const std::chrono::duration<Rep, Period>& timeout) const noexcept {
    std::unique_lock<std::mutex> _l(mMutex_);
    if (mComplete_.wait_for(_l, timeout, [this]() { return mCount_ == 0;})) {
      return true;
    }
    return false;
  }

 private:
  int32_t mCount_;
  mutable std::condition_variable mComplete_;
  mutable std::mutex mMutex_;
};

}  // namespace curl_transfer
}  // namespace connectivity
}  // namespace antioch

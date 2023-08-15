// Copyright Antioch. All rights reserved.

#include "latch.h"

namespace antioch {
namespace connectivity {
namespace curl_transfer {

void Latch::count_down(ptrdiff_t update) {
  std::unique_lock<std::mutex> _l(mMutex_);
  if (mCount_ == 0) {
    return;
  }
  if (mCount_ <= update) {
    mCount_ = 0;
    mComplete_.notify_all();
    return;
  }
  mCount_ -= update;
}

bool Latch::try_wait() const noexcept {
  std::unique_lock<std::mutex> _l(mMutex_);
  return mCount_ == 0;
}

void Latch::wait() const noexcept {
  std::unique_lock<std::mutex> _l(mMutex_);
  mComplete_.wait(_l, [this]() { return mCount_ == 0; });
}

void Latch::arrive_and_wait(ptrdiff_t update) noexcept {
  count_down(update);
  wait();
}

}  // namespace curl_transfer
}  // namespace connectivity
}  // namespace antioch

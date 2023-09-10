// Copyright Antioch. All rights reserved.

#pragma once

#include <stdexcept>

namespace antioch {
namespace connectivity {
namespace wifi {

enum class ErrorReason {
  ERROR_NO_SSID,
  ERROR_OTHER,
  ERROR_SOURCE_CODE,
};

std::string ErrorReason_name(ErrorReason reason);

class WifiConnectionError : public std::runtime_error {
 public:
  WifiConnectionError(ErrorReason what) : std::runtime_error(ErrorReason_name(what)), reason_(what) {}
  ErrorReason reason() { return reason_; }

 private:
  ErrorReason reason_;
};

}  // namespace wifi
}  // namespace connectivity
}  // namespace antioch

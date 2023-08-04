// Copyright Antioch. All rights reserved.

#pragma once

#include <stdexcept>
#include <string>

namespace antioch {
namespace connectivity {
namespace curl_transfer {

/**
 * Initiate an async curl transfer.
 *
 * @param url the url to curl
 * @param write_cb a function that takes as input the read data
 */
void start_transfer(const std::string& url, bool follow_redirects,
                    void (*cb)(std::string));

class LibCurlInternalException : public std::runtime_error {
 public:
  LibCurlInternalException(const std::string& what) : std::runtime_error(what) {}
};

}  // namespace curl_transfer
}  // namespace connectivity
}  // namespace antioch

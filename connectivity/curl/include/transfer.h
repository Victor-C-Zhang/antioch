// Copyright Antioch. All rights reserved.

#pragma once

#include <string>

namespace antioch {
namespace connectivity {
namespace curl_transfer {

void curl_init_internal();
void curl_destruct_internal();

/**
 * Initiate an async curl transfer.
 * 
 * @param url the url to curl
 * @param write_cb a function that writes data
 */
void start_transfer(const std::string& url, size_t (*write_cb)(char* ptr, size_t size, size_t nmemb, void* userdata));

class LibCurlInternalException : public std::runtime_error {
 public:
  LibCurlInternalException(const std::string& what) : std::runtime_error(what) {}
};

}  // namespace curl_transfer
}  // namespace connectivity
}  // namespace antioch

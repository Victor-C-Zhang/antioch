// Copyright Antioch. All rights reserved.

#include "transfer.h"

#include <curl/curl.h>

#include <iostream>
#include <memory>
#include <mutex>

#include "multi_performer.h"

namespace antioch::connectivity::curl_transfer {

static constexpr long CACHE_LIMIT = 5L;  // only allow so many cached connections

// Lazy-constructed singleton
class _Transfer_Internal {
 public:
  static _Transfer_Internal& instance() {
    static _Transfer_Internal instance;
    return instance;
  }
  _Transfer_Internal(const _Transfer_Internal&) = delete;
  void operator=(const _Transfer_Internal&) = delete;

  void start_transfer(const std::string& url, bool follow_redirects, std::function<void(std::string)> cb);

 private:
  _Transfer_Internal();
  ~_Transfer_Internal();
  void curl_init_internal();
  void curl_destruct_internal();

   bool global_inited = false;
   std::mutex global_init_mtx;

   CURLM* cm;
   MultiPerformer* mp;
};

_Transfer_Internal::_Transfer_Internal() {
  std::cout << "construct _transfer_internal" << std::endl;
  curl_init_internal();
}

_Transfer_Internal::~_Transfer_Internal() {
  std::cout << "destruct _transfer_internal" << std::endl;
  curl_destruct_internal();
}

void _Transfer_Internal::curl_init_internal() {
  std::scoped_lock<std::mutex> l(global_init_mtx);
  if (global_inited) {
    std::cout << "Already called curl_global_init" << std::endl;
    return;
  }

  auto res = curl_global_init(CURL_GLOBAL_ALL);
  if (res != 0) {
    std::string msg = "curl_global_init returned " + std::to_string(res);
    throw LibCurlInternalException(msg);
  }

  cm = curl_multi_init();
  if (cm == nullptr) {
    curl_global_cleanup();
    throw LibCurlInternalException("curl_multi_init returned nullptr");
  }
  curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, CACHE_LIMIT);
  mp = new MultiPerformer(cm);
  global_inited = true;
}

void _Transfer_Internal::curl_destruct_internal() {
  std::scoped_lock<std::mutex> l(global_init_mtx);
  if (!global_inited) {
    std::cout << "Already called curl_global_cleanup" << std::endl;
    return;
  }
  delete mp;
  curl_multi_cleanup(cm);
  curl_global_cleanup();
  global_inited = false;
}

void _Transfer_Internal::start_transfer(const std::string& url, bool follow_redirects, std::function<void(std::string)> cb) {
  CURL* eh = curl_easy_init();
  if (eh == nullptr) {
    throw LibCurlInternalException("curl_easy_init returned nullptr");
  }
  curl_easy_setopt(eh, CURLOPT_URL, url.c_str());
  if (follow_redirects) {
    curl_easy_setopt(eh, CURLOPT_FOLLOWLOCATION, 1L);
  }
  if (curl_easy_setopt(eh, CURLOPT_ACCEPT_ENCODING, "gzip") != CURLE_OK) {
    throw LibCurlInternalException("curl_easy_setopt encoding failed!");
  }
  mp->queue_transfer(eh, cb);
}

void start_transfer(const std::string& url, bool follow_redirects, std::function<void(std::string)> cb) {
  _Transfer_Internal::instance().start_transfer(url, follow_redirects, cb);
}

}  // namespace antioch::connectivity::curl_transfer

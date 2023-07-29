// Copyright Antioch. All rights reserved.

#include "transfer.h"

#include <curl/curl.h>

#include <iostream>
#include <memory>
#include <mutex>

#include "multi_performer.h"

namespace antioch::connectivity::curl_transfer {

typedef struct {
  size_t size;
  char* buf;
} curl_data;

static bool global_inited = false;
static std::mutex global_init_mtx;

static CURLM* cm;
static MultiPerformer* mp;

static constexpr long CACHE_LIMIT = 5L;  // only allow so many cached connections

void curl_init_internal() {
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
  mp = new MultiPerformer();
  global_inited = true;
}

void curl_destruct_internal() {
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

void start_transfer(const std::string& url,
                    size_t (*write_cb)(char* ptr, size_t size, size_t nmemb, void* userdata)) {
  curl_init_internal();
  CURL* eh = curl_easy_init();
  if (eh == nullptr) {
    throw LibCurlInternalException("curl_easy_init returned nullptr");
  }
  curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(eh, CURLOPT_URL, url.c_str());
  auto res = curl_multi_add_handle(cm, eh);
  if (res != CURLM_OK) {
    std::string msg = "curl_multi_add_handle returned " + std::to_string(res);
    throw LibCurlInternalException(msg);
  }
  std::unique_ptr<CURL> eh_ptr(eh);

}

}  // namespace antioch::connectivity::curl_transfer

// Copyright Antioch. All rights reserved.

#include "multi_performer.h"

#include <cstring>
#include <iostream>
#include <thread>

#include "transfer.h"

namespace antioch::connectivity::curl_transfer {

MultiPerformer::MultiPerformer(CURLM* multi_handle) : multi_handle(multi_handle) {
  std::thread thr(&MultiPerformer::run, this);
  thr.detach();
}

MultiPerformer::~MultiPerformer() {
  std::lock_guard<std::mutex> l(mtx);
  shutdown = true;
  cv.notify_all();
  for (const auto& pv : handle_objs) {
    curl_multi_remove_handle(multi_handle, pv.first);
  }
}

static size_t write_cb(void *data, size_t size, size_t nmemb, void *clientp) {
  size_t realsize = size * nmemb;
  auto mem = (EasyObj*)clientp;
  auto ptr = (char*)realloc(mem->buf, mem->size + realsize + 1);
  if(ptr == nullptr) {
    return 0;
  }
 
  mem->buf = ptr;
  std::memcpy(&(mem->buf[mem->size]), data, realsize);
  mem->size += realsize;
  mem->buf[mem->size] = 0; // null-terminate string
 
  return realsize;
}

void MultiPerformer::queue_transfer(CURL* easy_handle, void (*finish_cb)(std::unique_ptr<char>)) {
  std::scoped_lock<std::mutex> l(mtx);
  handle_objs[easy_handle] = {finish_cb, 0, nullptr};
  curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &handle_objs[easy_handle]);
  auto res = curl_multi_add_handle(multi_handle, easy_handle);
  if (res != CURLM_OK) {
    std::string msg = "curl_multi_add_handle returned " + std::to_string(res);
    handle_objs.erase(handle_objs.find(easy_handle));
    throw LibCurlInternalException(msg);
  }
  if (!flag) {
    flag = true;
    cv.notify_all();
  }
}

void MultiPerformer::run() {
  while (1) {
    {
      std::unique_lock<std::mutex> l(mtx);
      if (shutdown) {
        return;
      }
      if (!flag) {
        cv.wait(l, [this] { return flag; });
      }
      if (shutdown) {
        return;
      }
    }
    // potentailly multiple transfers chained together
    do_transfers();
    {
      std::scoped_lock<std::mutex> l(mtx);
      flag = false;
    }
  }
}

void MultiPerformer::do_transfers() {
  int still_running = 0;
  do {
    CURLMcode mc = curl_multi_perform(multi_handle, &still_running);
    if (mc) {
      std::string msg = "curl_multi_perform failed with " + std::to_string(mc);
      throw LibCurlInternalException(msg);
    }
    CURLMsg* read_res;
    int _;
    while ((read_res = curl_multi_info_read(multi_handle, &_))) {
      if (read_res->msg == CURLMSG_DONE) {
        curl_multi_remove_handle(multi_handle, read_res->easy_handle);
        curl_easy_cleanup(read_res->easy_handle);
        {
          std::scoped_lock<std::mutex> l(mtx);
          const auto& easy_obj = handle_objs[read_res->easy_handle];
          auto thr = std::thread(easy_obj.finish_cb, std::unique_ptr<char>(easy_obj.buf));
          thr.detach();
          handle_objs.erase(read_res->easy_handle);
        }
      } else {
        // TODO: do we need to remove and delete the easy_handle?
        std::cout << "DIAG: CurlMsg " << read_res->msg << std::endl;
        {
          std::scoped_lock<std::mutex> l(mtx);
          const auto& easy_obj = handle_objs[read_res->easy_handle];
          auto thr = std::thread(easy_obj.finish_cb, std::unique_ptr<char>(nullptr));
          thr.detach();
          handle_objs.erase(read_res->easy_handle);
        }
      }
    }
    if (still_running) {
      curl_multi_wait(multi_handle, nullptr, 0, 1000, nullptr);
    }
  } while (still_running);
}

}  // namespace antioch::connectivity::curl_transfer

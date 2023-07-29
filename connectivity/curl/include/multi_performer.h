// Copyright Antioch. All rights reserved.

#pragma once

#include <curl/curl.h>

#include <condition_variable>
#include <memory>
#include <mutex>
#include <vector>

namespace antioch {
namespace connectivity {
namespace curl_transfer {

class MultiPerformer {
 public:
  MultiPerformer(CURLM* multi_handle) : multi_handle(multi_handle) {}
  ~MultiPerformer();

  /**
   * Queues the transfer. The function will return immediately, but the transfer will happen in the
   * background.
   * Implementation detail: takes ownership of the passed easy_handle object and frees it after the
   * transfer is complete.
   */
  void queue_transfer(std::unique_ptr<CURL> easy_handle);

 private:
  void run();
  virtual void do_transfers();

  CURLM* multi_handle;

  std::mutex mtx;  // synchronizes all the below objects
  std::condition_variable cv;
  bool flag = false;
  std::vector<std::unique_ptr<CURL>> handle_objs;
};

}  // namespace curl_transfer
}  // namespace connectivity
}  // namespace antioch

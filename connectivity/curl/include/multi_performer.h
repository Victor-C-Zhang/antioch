// Copyright Antioch. All rights reserved.

#pragma once

#include <curl/curl.h>

#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>

namespace antioch {
namespace connectivity {
namespace curl_transfer {

typedef struct {
  std::function<void(std::string)> finish_cb;
  size_t size;
  char* buf;
} EasyObj;

class MultiPerformer {
 public:
  MultiPerformer(CURLM* multi_handle);
  virtual ~MultiPerformer();

  /**
   * Queues the transfer. The function will return immediately, but the transfer will happen in the
   * background. The finish_cb will be called when the transfer is finished or errors out.
   */
  void queue_transfer(CURL* easy_handle, std::function<void(std::string)> finish_cb);

 private:
  void run();
  virtual void do_transfers();

  CURLM* multi_handle;

  std::mutex mtx;  // synchronizes all the below objects
  std::condition_variable cv;
  bool more_to_transfer = false;
  bool shutdown = false;
  std::map<CURL*, EasyObj> handle_objs;
};

}  // namespace curl_transfer
}  // namespace connectivity
}  // namespace antioch

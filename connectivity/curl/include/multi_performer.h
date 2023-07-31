// Copyright Antioch. All rights reserved.

#pragma once

#include <curl/curl.h>

#include <condition_variable>
#include <memory>
#include <map>
#include <mutex>

namespace antioch {
namespace connectivity {
namespace curl_transfer {

typedef struct {
  void (*finish_cb)(std::unique_ptr<char>);
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
  void queue_transfer(CURL* easy_handle, void (*finish_cb)(std::unique_ptr<char>));

 private:
  void run();
  virtual void do_transfers();

  CURLM* multi_handle;

  std::mutex mtx;  // synchronizes all the below objects
  std::condition_variable cv;
  bool flag = false;
  bool shutdown = false;
  std::map<CURL*, EasyObj> handle_objs;
};

}  // namespace curl_transfer
}  // namespace connectivity
}  // namespace antioch

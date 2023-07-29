// Copyright Antioch. All rights reserved.

#include "multi_performer.h"

#include "transfer.h"

namespace antioch::connectivity::curl_transfer {

MultiPerformer::~MultiPerformer() {}

void MultiPerformer::queue_transfer(std::unique_ptr<CURL> easy_handle) {
  std::scoped_lock<std::mutex> l(mtx);
  handle_objs.push_back(easy_handle);
  if (!flag) {
    flag = true;
    cv.notify_all();
  }
}

void MultiPerformer::run() {
  while (1) {
    size_t num_transfers;
    {
      std::unique_lock<std::mutex> l(mtx);
      if (!flag) {
        cv.wait(l, [this] { return flag; });
      }
      num_transfers = handle_objs.size();
    }
    // potentailly multiple transfers chained together
    do_transfers();
    {
      std::scoped_lock<std::mutex> l(mtx);
      handle_objs.erase(handle_objs.begin(), handle_objs.begin() + num_transfers);
      flag = false;
    }
  }
}

void MultiPerformer::do_transfers() {
  int still_running;
  do {
    CURLMcode mc = curl_multi_perform(multi_handle, &still_running);

    if (!mc && still_running) {
      /* wait for activity, timeout or "nothing" */
      mc = curl_multi_poll(multi_handle, NULL, 0, 1000, NULL);
    }
    if (mc) {
      std::string msg = "curl_multi_poll failed with " + std::to_string(mc);
      throw LibCurlInternalException(msg);
      break;
    }
  } while (still_running);
}

}  // namespace antioch::connectivity::curl_transfer

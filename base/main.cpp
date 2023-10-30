// Copyright Antioch. All rights reserved.

#include <antioch/base/service.h>

int main() {
  auto service = antioch::base::Service::instance();
  service->start();
  service->spin();
  return 0;
}

// Copyright Antioch. All rights reserved.

#include <antioch/base/service.h>

int main() {
  auto service = antioch::base::Service();
  service.start();
  service.spin();
  return 0;
}

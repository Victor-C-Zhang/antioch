// Copyright Antioch. All rights reserved.

#include <SDL2/SDL.h>
#include <gfx/gfx.h>

#include <cassert>

using namespace antioch::gfx;

#define CHECK_ERROR(expr) assert(expr == Result::eSuccess);

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  InstanceCreateInfo instanceInfo{};
  Instance instance;
  CHECK_ERROR(createInstance(instanceInfo, nullptr, &instance));

  DeviceCreateInfo deviceInfo{};
  Device device;
  CHECK_ERROR(instance.createDevice(deviceInfo, nullptr, &device));

  CommandBufferCreateInfo cmdBufInfo{};
  CommandBuffer cmdBuf[2];
  CHECK_ERROR(device.createCommandBuffer(cmdBufInfo, nullptr, &cmdBuf[0]));
  CHECK_ERROR(device.createCommandBuffer(cmdBufInfo, nullptr, &cmdBuf[1]));

  CommandBufferBeginInfo beginInfo{};
  beginInfo.clearColour.r = 0xFF;
  CHECK_ERROR(cmdBuf[0].begin(beginInfo));
  CHECK_ERROR(cmdBuf[0].end());

  SubmitInfo submitInfo[2] = {SubmitInfo{.commandBufferCount = 1, .pCommandBuffers = &cmdBuf[0]},
                              SubmitInfo{.commandBufferCount = 1, .pCommandBuffers = &cmdBuf[1]}};

  CHECK_ERROR(device.submit(1, submitInfo));

  bool quit = false;
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_QUIT:
          quit = true;
          break;
      }
    }
  }

  for (uint32_t i = 0; i < 2; i++) {
    CHECK_ERROR(cmdBuf[i].destory());
  }

  CHECK_ERROR(device.destory());
  CHECK_ERROR(instance.destory());
  return 0;
}

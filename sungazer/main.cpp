// Copyright Antioch. All rights reserved.

#include <SDL2/SDL.h>
#include <gfx/gfx.h>

using namespace antioch::gfx;

int main(int argc, char *argv[]) {
  InstanceCreateInfo instanceInfo{};
  Instance instance;
  createInstance(instanceInfo, nullptr, &instance);

  DeviceCreateInfo deviceInfo{};
  Device device;
  instance.createDevice(deviceInfo, nullptr, &device);

  CommandBufferCreateInfo cmdBufInfo{};
  CommandBuffer cmdBuf[2];
  device.createCommandBuffer(cmdBufInfo, nullptr, &cmdBuf[0]);
  device.createCommandBuffer(cmdBufInfo, nullptr, &cmdBuf[1]);

  SubmitInfo submitInfo[2] = {SubmitInfo{.commandBufferCount = 1, .pCommandBuffers = &cmdBuf[0]},
                              SubmitInfo{.commandBufferCount = 1, .pCommandBuffers = &cmdBuf[1]}};

  device.submit(2, submitInfo);

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
    cmdBuf[i].destory();
  }

  device.destory();
  instance.destory();
  return 0;
}

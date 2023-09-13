// Copyright Antioch. All rights reserved.

#define MATRIX_WIDTH 64
#define MATRIX_HEIGHT 32
#define CHAIN_LENGTH 2

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "common/allocation.h"
#include "esp32/objects.h"

namespace antioch::gfx {

Device_t* implAllocateDevice(const AllocationCallback* pAllocator) {
  return antioch::gfx::common::allocate<ESP32Device_t>(pAllocator);
}

Result implCreateDevice(Device_t* baseDevice) {
  ESP32Device_t* device = static_cast<ESP32Device_t*>(baseDevice);

  device->display = nullptr;

  HUB75_I2S_CFG mxconfig;
  mxconfig.double_buff = true;

  device->display = new MatrixPanel_I2S_DMA(mxconfig);
  device->display->begin();

  return Result::eSuccess;
}

Result implDestroyDevice(Device_t* baseDevice, const AllocationCallback* pAllocator) {
  ESP32Device_t* device = static_cast<ESP32Device_t*>(baseDevice);

  antioch::gfx::common::deallocate<ESP32Device_t>(pAllocator, device);

  return Result::eSuccess;
}

Result implSubmit(Device_t* baseDevice, uint32_t submitCount, const SubmitInfo* pSubmits) {
  ESP32Device_t* device = static_cast<ESP32Device_t*>(baseDevice);
  device->display->flipDMABuffer();
  device->display->clearScreen();

  for (uint32_t n = 0; n < submitCount; n++) {
    RenderTarget renderTarget = pSubmits[n].renderTarget;
    uint32_t width = renderTarget->createInfo.extents.x;
    uint32_t height = renderTarget->createInfo.extents.y;

    uint32_t currCol = 0;
    uint32_t currColCount = 0;

    for (uint32_t j = 0; j < height; j++) {
      for (uint32_t i = 0; i < width; i++) {
        uint8_t r = renderTarget->screen[j * width * 3 + i * 3];
        uint8_t g = renderTarget->screen[j * width * 3 + i * 3 + 1];
        uint8_t b = renderTarget->screen[j * width * 3 + i * 3 + 2];

        if ((r | g << 8 | b << 16) == currCol) {
          currColCount++;
        } else {
          if (currCol != 0) {
            device->display->drawFastHLine(i - currColCount, j, currColCount, currCol & 0xFF,
                                           (currCol >> 8) & 0xFF, (currCol >> 16) & 0xFF);
          }

          currCol = r | g << 8 | b << 16;
          currColCount = 1;
        }

        // Flush rest
        if (i == width - 1) {
          device->display->drawFastHLine(i - currColCount + 1, j, currColCount, r, g, b);
        }
      }
    }
  }

  return Result::eSuccess;
}

}  // namespace antioch::gfx

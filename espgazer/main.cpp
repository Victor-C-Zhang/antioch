/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <gfx/gfx.h>
#include <gfx/gfx_helpers.h>
#include <inttypes.h>

#include <cstdio>
#include <cstring>
#include <string>

#include "esp_chip_info.h"
#include "esp_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

using namespace antioch::gfx;
#define CHECK_ERROR(expr)         \
  if (expr != Result::eSuccess) { \
    printf("%s failed\n", #expr); \
  }

extern "C" void app_main(void) {
  printf("Hello world!\n");

  /* Print chip information */
  esp_chip_info_t chip_info;
  uint32_t flash_size;
  esp_chip_info(&chip_info);
  printf("This is %s chip with %d CPU core(s), %s%s%s%s, ", CONFIG_IDF_TARGET, chip_info.cores,
         (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
         (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
         (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
         (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

  unsigned major_rev = chip_info.revision / 100;
  unsigned minor_rev = chip_info.revision % 100;
  printf("silicon revision v%d.%d, ", major_rev, minor_rev);
  if (esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
    printf("Get flash size failed");
    return;
  }

  printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
         (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

  InstanceCreateInfo instanceInfo{};
  Instance instance;
  CHECK_ERROR(createInstance(instanceInfo, nullptr, &instance));

  DeviceCreateInfo deviceInfo{};
  Device device;
  CHECK_ERROR(instance.createDevice(deviceInfo, nullptr, &device));

  RenderTargetCreateInfo renderTargetInfo{};
  renderTargetInfo.extents = {.x = SCREEN_WIDTH, .y = SCREEN_HEIGHT};
  renderTargetInfo.numChannels = 3;
  RenderTarget renderTarget;
  CHECK_ERROR(device.createRenderTarget(renderTargetInfo, nullptr, &renderTarget));

  CommandBufferCreateInfo cmdBufInfo{};
  CommandBuffer cmdBuf;
  CHECK_ERROR(device.createCommandBuffer(cmdBufInfo, nullptr, &cmdBuf));

  MemoryAllocateInfo allocInfo{};
  allocInfo.allocationSize = sizeof(default_glyphs::kFonts);

  DeviceMemory memory;
  CHECK_ERROR(device.allocateMemory(allocInfo, nullptr, &memory));

  printf("Heap size: %" PRIu32 " bytes\n", esp_get_free_heap_size());

  uint8_t* pMemory = nullptr;
  CHECK_ERROR(device.mapMemory(memory, reinterpret_cast<void**>(&pMemory)));
  memcpy(pMemory, default_glyphs::kFonts, sizeof(default_glyphs::kFonts));
  printf("pMemory %p\n", pMemory);

  // for (uint32_t i = 0; i < sizeof(default_glyphs::kFonts); i++) {
  //   printf("i=%" PRIu32 " v=%" PRIX32 "\n", i, (uint32_t)pMemory[i]);
  // }
  BufferCreateInfo bufferInfo{};
  bufferInfo.size = sizeof(default_glyphs::kFonts);

  Buffer buffer;
  CHECK_ERROR(device.createBuffer(bufferInfo, nullptr, &buffer));
  CHECK_ERROR(device.bindBuffer(memory, buffer, 0));

  GlyphCreateInfo glyphInfo{};
  glyphInfo.isColMajor = true;
  glyphInfo.buffer = buffer;
  glyphInfo.strideBytes = 1;
  glyphInfo.extent.x = 5;
  glyphInfo.extent.y = 7;

  Glyph glyphs[255];

  for (uint32_t i = 0; i < 255; i++) {
    glyphInfo.value = i;
    glyphInfo.bufferOffset = i * 5 * sizeof(uint8_t);
    CHECK_ERROR(device.createGlyph(glyphInfo, nullptr, &glyphs[i]));
  }

  BrushCreateInfo brushInfo{};
  brushInfo.brushColour = {.r = 0x00, .g = 0xFF, .b = 0x00};
  Brush brush;
  CHECK_ERROR(device.createBrush(brushInfo, nullptr, &brush));

  CommandBufferBeginInfo beginInfo{};
  beginInfo.clearColour.r = 0x10;
  beginInfo.clearColour.g = 0xff;
  beginInfo.clearColour.b = 0xFF;
  CHECK_ERROR(cmdBuf.reset());
  CHECK_ERROR(cmdBuf.begin(beginInfo));
  CHECK_ERROR(cmdBuf.bindBrush(brush));
  CHECK_ERROR(cmdBuf.bindGlyph(255, glyphs));

  std::string text1 = "Hello...";
  std::string text2 = "world?";
  std::string text3 = "@antioch";

  uint32_t x = 32;

  CHECK_ERROR(cmdBuf.drawText(text1.c_str(), text1.size(), {.x = 64 - x, .y = 1}));
  CHECK_ERROR(cmdBuf.drawText(text2.c_str(), text2.size(), {.x = 64 - x, .y = 9}));
  CHECK_ERROR(cmdBuf.drawText(text3.c_str(), text3.size(), {.x = 64 - x, .y = 17}));

  CHECK_ERROR(cmdBuf.end());

  printf("End command buffer recording\n");

  SubmitInfo submitInfo = {
      .renderTarget = renderTarget, .commandBufferCount = 1, .pCommandBuffers = &cmdBuf};

  for (uint32_t frame = 0; frame < 500; frame++) {
    x = (x + 1) % 128;
    CHECK_ERROR(cmdBuf.reset());
    CHECK_ERROR(cmdBuf.begin(beginInfo));
    CHECK_ERROR(cmdBuf.bindBrush(brush));
    CHECK_ERROR(cmdBuf.bindGlyph(255, glyphs));

    CHECK_ERROR(cmdBuf.drawText(text1.c_str(), text1.size(), {.x = 64 - x, .y = 1}));
    CHECK_ERROR(cmdBuf.drawText(text2.c_str(), text2.size(), {.x = 64 - x, .y = 9}));
    CHECK_ERROR(cmdBuf.drawText(text3.c_str(), text3.size(), {.x = 64 - x, .y = 17}));

    CHECK_ERROR(cmdBuf.end());

    CHECK_ERROR(device.submit(1, &submitInfo));
    printf("Waiting 10ms....\n");
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }

  CHECK_ERROR(device.destroyBrush(brush));

  for (uint32_t i = 0; i < 255; i++) {
    CHECK_ERROR(device.destroyGlyph(glyphs[i]));
  }

  CHECK_ERROR(device.destroyBuffer(buffer));

  CHECK_ERROR(device.freeMemory(memory));

  CHECK_ERROR(cmdBuf.destory());
  CHECK_ERROR(device.destroy());
  CHECK_ERROR(instance.destroy());

  for (int i = 10; i >= 0; i--) {
    printf("Restarting in %d seconds...\n", i);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  printf("Restarting now.\n");

  fflush(stdout);
  esp_restart();
}

/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <gfx/gfx.h>
#include <gfx/gfx_helpers.h>
#include <inttypes.h>

#include <cstring>
#include <string>

#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

using namespace antioch::gfx;
#define CHECK_ERROR(expr)                       \
  if (expr != Result::eSuccess) {               \
    ESP_LOGE("espgazer", "%s failed\n", #expr); \
    abort();                                    \
  }

void gazer_main() {
  printf("Hello world!\n");

  /* Print chip information */
  esp_chip_info_t chip_info;
  uint32_t flash_size;
  esp_chip_info(&chip_info);
  ESP_LOGI("espgazer", "This is %s chip with %d CPU core(s), %s%s%s%s, ", CONFIG_IDF_TARGET,
           chip_info.cores, (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

  unsigned major_rev = chip_info.revision / 100;
  unsigned minor_rev = chip_info.revision % 100;
  ESP_LOGI("espgazer", "silicon revision v%d.%d, ", major_rev, minor_rev);
  if (esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
    ESP_LOGI("espgazer", "Get flash size failed");
    return;
  }

  ESP_LOGI("espgazer", "%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  ESP_LOGI("espgazer", "Minimum free heap size: %" PRIu32 " bytes\n",
           esp_get_minimum_free_heap_size());

  InstanceCreateInfo instanceInfo{};
  Instance instance;
  CHECK_ERROR(createInstance(instanceInfo, nullptr, &instance));

  DeviceCreateInfo deviceInfo{};
  Device device;
  CHECK_ERROR(instance.createDevice(deviceInfo, nullptr, &device));

  RenderTargetCreateInfo renderTargetInfo{};
  renderTargetInfo.extents = {.x = 128, .y = 32};
  renderTargetInfo.numChannels = 3;
  renderTargetInfo.edgeBehaviour = EdgeBehaviour::eWrap;
  RenderTarget renderTarget;
  CHECK_ERROR(device.createRenderTarget(renderTargetInfo, nullptr, &renderTarget));

  CommandBufferCreateInfo cmdBufInfo{};
  CommandBuffer cmdBuf;
  CHECK_ERROR(device.createCommandBuffer(cmdBufInfo, nullptr, &cmdBuf));

  MemoryAllocateInfo allocInfo{};
  allocInfo.allocationSize = sizeof(default_glyphs::kFonts);

  DeviceMemory memory;
  CHECK_ERROR(device.allocateMemory(allocInfo, nullptr, &memory));

  ESP_LOGI("espgazer", "Heap size: %" PRIu32 " bytes\n", esp_get_free_heap_size());

  uint8_t* pMemory = nullptr;
  CHECK_ERROR(device.mapMemory(memory, reinterpret_cast<void**>(&pMemory)));
  memcpy(pMemory, default_glyphs::kFonts, sizeof(default_glyphs::kFonts));
  ESP_LOGI("espgazer", "pMemory %p\n", pMemory);

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

  const uint32_t numColours = 8;
  uint32_t colours[numColours];

  for (uint32_t i = 0; i < numColours; i++) {
    colours[i] = 0;
    for (uint32_t j = 0; j < 3; j++) {
      if (i & (1 << j)) {
        colours[i] |= 0xFF << (j * 8);
      }
    }
  }

  BrushCreateInfo brushInfo{};
  Brush brushes[8];
  for (uint32_t i = 0; i < numColours; i++) {
    brushInfo.brushColour = {.r = static_cast<uint8_t>((colours[i]) & 0xFF),
                             .g = static_cast<uint8_t>((colours[i] >> 8) & 0xFF),
                             .b = static_cast<uint8_t>((colours[i] >> 16) & 0xFF)};
    CHECK_ERROR(device.createBrush(brushInfo, nullptr, &brushes[i]));
  }

  CommandBufferBeginInfo begins[8];

  for (uint32_t i = 0; i < numColours; i++) {
    begins[i] = {
        .clearColour = {.r = static_cast<uint8_t>((colours[numColours - i - 1]) & 0xFF),
                        .g = static_cast<uint8_t>((colours[numColours - i - 1] >> 8) & 0xFF),
                        .b = static_cast<uint8_t>((colours[numColours - i - 1] >> 16) & 0xFF)}};
  }
  std::string text[3] = {"Hello...", "world?", "#IYKYK"};
  Vector2D pos[3] = {{.x = 128, .y = 4}, {.x = 128, .y = 12}, {.x = 128, .y = 20}};
  SubmitInfo submitInfo = {
      .renderTarget = renderTarget, .commandBufferCount = 1, .pCommandBuffers = &cmdBuf};

  const uint32_t bpm = 126;
  const uint32_t fpb = 4;
  const uint32_t durationMin = 5;
  const auto Hz = (60 * configTICK_RATE_HZ) / (fpb * bpm);
  uint32_t frame = 0;
  for (; frame < fpb * bpm * durationMin;) {
    frame = (frame + 1) % (fpb * bpm * durationMin);
    uint32_t brushColIdx = (frame / (fpb)) % 2 == 0 ? 2 : 0;
    uint32_t clearColIdx = (frame / (fpb)) % 2 == 0 ? 7 : 5;
    uint32_t colIdx = (frame / (fpb)) % numColours;

    uint32_t scene = frame / (fpb * bpm);
    scene = 1;
    Vector2D speed = {.x = -2, .y = 0};
    Vector2D offset = speed * ((frame) / fpb);

    CHECK_ERROR(cmdBuf.reset());
    CHECK_ERROR(cmdBuf.begin(begins[colIdx]));
    CHECK_ERROR(cmdBuf.bindBrush(brushes[colIdx]));
    CHECK_ERROR(cmdBuf.bindGlyph(255, glyphs));

    switch (scene) {
      case 1: {
        for (uint32_t i = 0; i < 3; i++) {
          CHECK_ERROR(cmdBuf.drawText(text[2].c_str(), text[2].size(),
                                      pos[i] + offset * ((i & 1) == 0 ? -1 : 1)));

          CHECK_ERROR(cmdBuf.drawText(text[2].c_str(), text[2].size(),
                                      pos[i] + speed * 32 + offset * ((i & 1) == 0 ? -1 : 1)));
        }
        break;
      }
      default: {
        for (uint32_t i = 0; i < 3; i++) {
          CHECK_ERROR(cmdBuf.drawText(text[i].c_str(), text[i].size(), pos[i] + offset));
        }
        break;
      }
    }

    CHECK_ERROR(cmdBuf.end());

    CHECK_ERROR(device.submit(1, &submitInfo));

    vTaskDelay(Hz);
  }

  for (uint32_t i = 0; i < numColours; i++) {
    CHECK_ERROR(device.destroyBrush(brushes[i]));
  }
  for (uint32_t i = 0; i < 255; i++) {
    CHECK_ERROR(device.destroyGlyph(glyphs[i]));
  }

  CHECK_ERROR(device.destroyBuffer(buffer));

  CHECK_ERROR(device.freeMemory(memory));

  CHECK_ERROR(cmdBuf.destory());
  CHECK_ERROR(device.destroy());
  CHECK_ERROR(instance.destroy());

  for (int i = 10; i >= 0; i--) {
    ESP_LOGI("espgazer", "Restarting in %d seconds...\n", i);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  ESP_LOGI("espgazer", "Restarting now.\n");

  fflush(stdout);
  esp_restart();
}

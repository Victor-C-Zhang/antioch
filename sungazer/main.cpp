// Copyright Antioch. All rights reserved.

#include <SDL2/SDL.h>
#include <gfx/gfx.h>
#include <gfx/gfx_helpers.h>

#include <cassert>
#include <string>

using namespace antioch::gfx;

#define CHECK_ERROR(expr) assert(expr == Result::eSuccess);

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
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

  MemoryAllocateInfo allocInfo{};
  allocInfo.allocationSize = sizeof(default_glyphs::kFonts);

  DeviceMemory memory;
  CHECK_ERROR(device.allocateMemory(allocInfo, nullptr, &memory));

  uint8_t* pMemory = nullptr;
  CHECK_ERROR(device.mapMemory(memory, reinterpret_cast<void**>(&pMemory)));
  memcpy(pMemory, default_glyphs::kFonts, sizeof(default_glyphs::kFonts));

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
  beginInfo.clearColour.r = 0x80;
  beginInfo.clearColour.g = 0x80;
  beginInfo.clearColour.b = 0x80;
  CHECK_ERROR(cmdBuf[0].reset());
  CHECK_ERROR(cmdBuf[0].begin(beginInfo));
  CHECK_ERROR(cmdBuf[0].bindBrush(brush));
  CHECK_ERROR(cmdBuf[0].bindGlyph(255, glyphs));

  std::string text1 = "Hello...";
  std::string text2 = "world?";
  std::string text3 = "@antioch";

  CHECK_ERROR(cmdBuf[0].drawText(text1.c_str(), text1.size(), {.x = 1, .y = 1}));
  CHECK_ERROR(cmdBuf[0].drawText(text2.c_str(), text2.size(), {.x = 1, .y = 9}));
  CHECK_ERROR(cmdBuf[0].drawText(text3.c_str(), text3.size(), {.x = 1, .y = 17}));

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

  CHECK_ERROR(device.destroyBrush(brush));

  for (uint32_t i = 0; i < 255; i++) {
    CHECK_ERROR(device.destroyGlyph(glyphs[i]));
  }

  CHECK_ERROR(device.destroyBuffer(buffer));

  CHECK_ERROR(device.freeMemory(memory));

  for (uint32_t i = 0; i < 2; i++) {
    CHECK_ERROR(cmdBuf[i].destory());
  }

  CHECK_ERROR(device.destroy());
  CHECK_ERROR(instance.destroy());
  return 0;
}

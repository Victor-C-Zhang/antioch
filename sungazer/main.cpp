// Copyright Antioch. All rights reserved.

#include <SDL2/SDL.h>
#include <gfx/gfx.h>
#include <gfx/gfx_helpers.h>

#include <cassert>
#include <chrono>
#include <iostream>
#include <string>

using namespace antioch::gfx;

using namespace std::chrono_literals;

#define CHECK_ERROR(expr) assert(expr == Result::eSuccess);

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
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
  brushInfo.brushColour = {.r = 0xFF, .g = 0xFF, .b = 0xFF};
  Brush brush;
  CHECK_ERROR(device.createBrush(brushInfo, nullptr, &brush));

  CommandBufferBeginInfo beginInfo{};
  beginInfo.clearColour.r = 0x00;
  beginInfo.clearColour.g = 0x00;
  beginInfo.clearColour.b = 0x00;

  std::string text1 = "Hello...";
  std::string text2 = "world?";
  std::string text3 = "@antioch";

  int32_t x = 128;

  SubmitInfo submitInfo = {
      .renderTarget = renderTarget, .commandBufferCount = 1, .pCommandBuffers = &cmdBuf};

  bool quit = false;

  auto lastFrame = std::chrono::high_resolution_clock::now();

  SDL_Event e;
  while (!quit) {
    const auto thisFrame = std::chrono::high_resolution_clock::now();
    if (thisFrame - lastFrame > 100ms) {
      x = (x + 1) % 256;
      CHECK_ERROR(cmdBuf.reset());
      CHECK_ERROR(cmdBuf.begin(beginInfo));
      CHECK_ERROR(cmdBuf.bindBrush(brush));
      CHECK_ERROR(cmdBuf.bindGlyph(255, glyphs));

      CHECK_ERROR(cmdBuf.drawText(text1.c_str(), text1.size(), {.x = 128 - x, .y = 1}));
      CHECK_ERROR(cmdBuf.drawText(text2.c_str(), text2.size(), {.x = 128 - x, .y = 9}));
      CHECK_ERROR(cmdBuf.drawText(text3.c_str(), text3.size(), {.x = 128 - x, .y = 17}));

      CHECK_ERROR(cmdBuf.end());

      CHECK_ERROR(device.submit(1, &submitInfo));

      lastFrame = thisFrame;
    }
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

  CHECK_ERROR(cmdBuf.destory());
  CHECK_ERROR(device.destroy());
  CHECK_ERROR(instance.destroy());
  return 0;
}

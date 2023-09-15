// Copyright Antioch. All rights reserved.

#include <SDL2/SDL.h>
#include <gfx/gfx.h>
#include <gfx/gfx_helpers.h>

#include <cassert>
#include <chrono>
#include <iomanip>
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
  std::string text[3] = {"Hello...", "world?", "@antioch"};
  Vector2D pos[3] = {{.x = 128, .y = 1}, {.x = 128, .y = 9}, {.x = 128, .y = 17}};
  SubmitInfo submitInfo = {
      .renderTarget = renderTarget, .commandBufferCount = 1, .pCommandBuffers = &cmdBuf};

  bool quit = false;

  auto lastFrame = std::chrono::high_resolution_clock::now();

  const uint32_t bpm = 128;
  const uint32_t fpb = 4;
  const uint32_t durationMin = 5;
  const auto Hz = std::chrono::duration_cast<std::chrono::microseconds>(60s) / (fpb * bpm);
  std::cout << "Hz="
            << (double)std::chrono::duration_cast<std::chrono::microseconds>(Hz).count() / 1000
            << "ms" << std::endl;
  uint32_t frame = 0;

  SDL_Event e;
  while (!quit) {
    const auto thisFrame = std::chrono::high_resolution_clock::now();
    const auto elapsed = thisFrame - lastFrame;
    if (elapsed > Hz) {
      frame = (frame + 1) % (fpb * bpm * durationMin);
      uint32_t colIdx = (frame / (fpb)) % numColours;

      Vector2D speed = {.x = -1, .y = 0};
      Vector2D offset = speed * ((frame * 2) / fpb);

      CHECK_ERROR(cmdBuf.reset());
      CHECK_ERROR(cmdBuf.begin(begins[colIdx]));
      CHECK_ERROR(cmdBuf.bindBrush(brushes[colIdx]));
      CHECK_ERROR(cmdBuf.bindGlyph(255, glyphs));

      for (uint32_t i = 0; i < 3; i++) {
        CHECK_ERROR(cmdBuf.drawText(text[i].c_str(), text[i].size(), pos[i] + offset));
      }

      CHECK_ERROR(cmdBuf.end());

      CHECK_ERROR(device.submit(1, &submitInfo));

      std::cout << "frame=" << frame << " colIdx=" << colIdx << " offset.x=" << offset.x
                << " offset.y=" << offset.y << std::hex << " clearCol=0x" << std::setw(8)
                << std::setfill('0') << colours[colIdx] << " brushCol=0x" << std::setw(8)
                << std::setfill('0') << colours[numColours - colIdx - 1] << std::dec << std::endl;

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
  return 0;
}

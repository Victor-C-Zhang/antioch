// Copyright Antioch. All rights reserved.

#include <SDL2/SDL.h>
#include <gfx/gfx.h>

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
  allocInfo.allocationSize = 3 * sizeof(default_glyphs::kL);

  DeviceMemory memory;
  CHECK_ERROR(device.allocateMemory(allocInfo, nullptr, &memory));

  uint8_t* pMemory = nullptr;
  CHECK_ERROR(device.mapMemory(memory, reinterpret_cast<void**>(&pMemory)));
  memcpy(pMemory, default_glyphs::kL, sizeof(default_glyphs::kL));
  memcpy(pMemory + sizeof(default_glyphs::kL), default_glyphs::kE, sizeof(default_glyphs::kE));
  memcpy(pMemory + sizeof(default_glyphs::kL) * 2, default_glyphs::kO, sizeof(default_glyphs::kO));

  ImageCreateInfo imageInfo{};
  imageInfo.format = ImageFormat::eA1Uint;
  imageInfo.extents = {.x = 8, .y = 8};

  Image images[3];
  for (uint32_t i = 0; i < 3; ++i) {
    CHECK_ERROR(device.createImage(imageInfo, nullptr, &images[i]));
    CHECK_ERROR(device.bindImage(memory, images[i], i * sizeof(default_glyphs::kL)));
  }

  GlyphCreateInfo glyphInfo{};

  Glyph glyphs[3];
  glyphInfo.image = images[0];
  glyphInfo.value = 'L';
  CHECK_ERROR(device.createGlyph(glyphInfo, nullptr, &glyphs[0]));
  glyphInfo.image = images[1];
  glyphInfo.value = 'E';
  CHECK_ERROR(device.createGlyph(glyphInfo, nullptr, &glyphs[1]));
  glyphInfo.image = images[2];
  glyphInfo.value = 'O';
  CHECK_ERROR(device.createGlyph(glyphInfo, nullptr, &glyphs[2]));

  BrushCreateInfo brushInfo{};
  brushInfo.brushColour = {.r = 0x00, .g = 0xFF, .b = 0x00};
  Brush brush;
  CHECK_ERROR(device.createBrush(brushInfo, nullptr, &brush));

  CommandBufferBeginInfo beginInfo{};
  beginInfo.clearColour.r = 0xFF;
  CHECK_ERROR(cmdBuf[0].reset());
  CHECK_ERROR(cmdBuf[0].begin(beginInfo));
  CHECK_ERROR(cmdBuf[0].bindBrush(brush));
  CHECK_ERROR(cmdBuf[0].bindGlyph(3, glyphs));

  std::string text = "LEOOOOOOOEE";

  CHECK_ERROR(cmdBuf[0].drawText(text.c_str(), text.size(), {.x = 1, .y = 1}));
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

  for (uint32_t i = 0; i < 3; i++) {
    CHECK_ERROR(device.destroyGlyph(glyphs[i]));
    CHECK_ERROR(device.destroyImage(images[i]));
  }

  CHECK_ERROR(device.freeMemory(memory));

  for (uint32_t i = 0; i < 2; i++) {
    CHECK_ERROR(cmdBuf[i].destory());
  }

  CHECK_ERROR(device.destroy());
  CHECK_ERROR(instance.destroy());
  return 0;
}

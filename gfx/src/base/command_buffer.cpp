// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "base/object_base.h"

namespace antioch::gfx {

GFX_API Result CommandBuffer::begin(const CommandBufferBeginInfo& beginInfo) {
  commandBuffer->clearColour = beginInfo.clearColour;

  return Result::eSuccess;
}

GFX_API Result CommandBuffer::bindBrush(const Brush& brush) {
  commandBuffer->state.brush = brush;

  return Result::eSuccess;
}

GFX_API Result CommandBuffer::bindGlyph(uint32_t glpyhCount, const Glyph* pGlyphs) {
  for (uint32_t i = 0; i < glpyhCount; ++i) {
    commandBuffer->state.font[pGlyphs[i]->createInfo.value] = pGlyphs[i];
  }

  return Result::eSuccess;
}

GFX_API Result CommandBuffer::drawText(const char* text, uint32_t textCount,
                                       const Vector2D& origin) {
  CommandBuffer_t::DrawData& currDraw =
      commandBuffer->state.drawData[commandBuffer->state.numDraws];
  currDraw.origin = origin;
  currDraw.numPrims = textCount;
  currDraw.brush = commandBuffer->state.brush;

  Vector2D offset = {.x = 6, .y = 0};

  for (uint32_t i = 0; i < textCount; ++i) {
    currDraw.prims[i].glyph = commandBuffer->state.font[static_cast<uint8_t>(text[i])];
    if (i > 0) {
      currDraw.prims[i].offset = offset;
    } else {
      currDraw.prims[i].offset = Vector2D{.x = 0, .y = 0};
    }
  }

  commandBuffer->state.numDraws++;

  return Result::eSuccess;
}

GFX_API Result CommandBuffer::end() { return Result::eSuccess; }

GFX_API Result CommandBuffer::reset() {
  commandBuffer->state.brush = nullptr;
  for (uint32_t i = 0; i < sizeof(commandBuffer->state.font) / sizeof(commandBuffer->state.font[0]);
       ++i) {
    commandBuffer->state.font[i] = nullptr;
  }

  commandBuffer->state.numDraws = 0;
  return Result::eSuccess;
}

GFX_API Result CommandBuffer::destory(const AllocationCallback* pAllocator) {
  return implDestroyCommandBuffer(commandBuffer, pAllocator);
}

}  // namespace antioch::gfx

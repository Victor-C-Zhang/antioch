// Copyright Antioch. All rights reserved.

#include <gfx/gfx.h>

#include "base/function_impl.h"
#include "base/object_base.h"

namespace antioch::gfx {

GFX_API Result CommandBuffer::begin(const CommandBufferBeginInfo& beginInfo) {
  commandBuffer->clearColour = beginInfo.clearColour;

  return Result::eSuccess;
}

GFX_API Result CommandBuffer::bindBrush(const BrushInfo& brushInfo) {
  commandBuffer->state.brush = brushInfo;

  return Result::eSuccess;
}

GFX_API Result CommandBuffer::bindGlyph(const GlyphInfo& fontInfo) {
  // TODO: Allocate fonts in buffers so we aren't copying every time
  commandBuffer->state.font = fontInfo;

  return Result::eSuccess;
}

GFX_API Result CommandBuffer::drawText(const char* text, size_t textSize) {
  return Result::eSuccess;
}

GFX_API Result CommandBuffer::end() { return Result::eSuccess; }

GFX_API Result CommandBuffer::destory(const AllocationCallback* pAllocator) {
  return implDestroyCommandBuffer(commandBuffer, pAllocator);
}

}  // namespace antioch::gfx

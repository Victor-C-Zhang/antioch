#include <gfx/gfx.h>
#include <gtest/gtest.h>

#include "test_objects.h"

using namespace antioch::gfx;

#define CHECK_ERROR(expr) ASSERT_EQ(expr, Result::eSuccess);

template <class T>
void expect_vector_eq(std::vector<T> a, std::vector<T> b) {
  ASSERT_EQ(a.size(), b.size()) << "Vector a and b are of unequal length";

  for (uint32_t i = 0; i < a.size(); ++i) {
    EXPECT_EQ(a[i], b[i]) << "Vector a and b differ at index " << i;
  }
}

TEST(GFX, ClearScreen) {
  InstanceCreateInfo instanceInfo{};
  Instance instance;
  CHECK_ERROR(createInstance(instanceInfo, nullptr, &instance));

  DeviceCreateInfo deviceInfo{};
  Device device;
  CHECK_ERROR(instance.createDevice(deviceInfo, nullptr, &device));

  RenderTargetCreateInfo renderTargetInfo{};
  renderTargetInfo.extents = {.x = 8, .y = 8};
  renderTargetInfo.numChannels = 3;
  RenderTarget renderTarget;
  CHECK_ERROR(device.createRenderTarget(renderTargetInfo, nullptr, &renderTarget));

  CommandBufferCreateInfo cmdBufInfo{};
  CommandBuffer cmdBuf;
  CHECK_ERROR(device.createCommandBuffer(cmdBufInfo, nullptr, &cmdBuf));

  CommandBufferBeginInfo beginInfo{};
  beginInfo.clearColour.r = 0x80;
  beginInfo.clearColour.g = 0x80;
  beginInfo.clearColour.b = 0x80;
  CHECK_ERROR(cmdBuf.reset());
  CHECK_ERROR(cmdBuf.begin(beginInfo));
  CHECK_ERROR(cmdBuf.end());

  SubmitInfo submitInfo{};
  submitInfo.renderTarget = renderTarget;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &cmdBuf;

  CHECK_ERROR(device.submit(1, &submitInfo));

  EXPECT_EQ(gTestData.numFramesSubmitted, 1);

  std::vector<uint8_t> expected(8 * 8 * 3, 0x80);

  expect_vector_eq(gTestData.framesSubmitted[0], expected);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

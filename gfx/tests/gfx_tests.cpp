#include <gfx/gfx.h>
#include <gtest/gtest.h>

#include "test_objects.h"

using namespace antioch::gfx;

#define CHECK_ERROR(expr) ASSERT_EQ(expr, Result::eSuccess);

TEST(GFX, ClearScreen) {
  InstanceCreateInfo instanceInfo{};
  Instance instance;
  CHECK_ERROR(createInstance(instanceInfo, nullptr, &instance));

  DeviceCreateInfo deviceInfo{};
  Device device;
  CHECK_ERROR(instance.createDevice(deviceInfo, nullptr, &device));

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
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &cmdBuf;

  CHECK_ERROR(device.submit(1, &submitInfo));

  EXPECT_EQ(gTestData.numFramesSubmitted, 1);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

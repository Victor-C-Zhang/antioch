// Copyright Antioch. All rights reserved.

#include <gtest/gtest.h>

TEST(Trival, Trival) { ASSERT_EQ(true, true); }

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

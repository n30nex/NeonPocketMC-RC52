#include <gtest/gtest.h>
#include "../../examples/companion_radio/ui-new/NeonPocketSplash.h"

static_assert(NeonPocketSplash::FRAME_MILLIS == 80, "splash cadence changed");
static_assert(NeonPocketSplash::DURATION_MILLIS == 3200, "splash duration changed");
static_assert(NeonPocketSplash::FRAME_COUNT == 40, "splash frame count changed");

TEST(NeonPocketSplash, Contract) {
  EXPECT_STREQ(NeonPocketSplash::BRAND, "NEONPOCKETMC");
  EXPECT_STREQ(NeonPocketSplash::ROLE, "MESHCORE COMPANION");
  EXPECT_EQ(NeonPocketSplash::frameForElapsed(3199), 39);
  EXPECT_EQ(NeonPocketSplash::frameForElapsed(3200), 39);
  EXPECT_EQ(NeonPocketSplash::frameForElapsed(999999), 39);
  EXPECT_STREQ(NeonPocketSplash::statusForFrame(39), "READY");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

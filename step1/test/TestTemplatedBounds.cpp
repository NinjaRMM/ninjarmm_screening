#include <gtest/gtest.h>
#include "../src/TemplatedBounds.hpp" 

TEST(TemplatedBounds, RecorgnizesUnsignedIntWithinBounds) {
  const uint32_t value = 520;
  const uint32_t min = 500;
  const uint32_t max = 599;
  EXPECT_TRUE(IsInBounds(value, min, max));
}

TEST(TemplatedBounds, RecorgnizesUnsignedIntAtMinimalBound) {
  const uint32_t value = 500;
  const uint32_t min = 500;
  const uint32_t max = 599;
  EXPECT_TRUE(IsInBounds(value, min, max));
}

TEST(TemplatedBounds, RecorgnizesUnsignedIntAtMaximalBound) {
  const uint32_t value = 599;
  const uint32_t min = 500;
  const uint32_t max = 599;
  EXPECT_TRUE(IsInBounds(value, min, max));
}

TEST(TemplatedBounds, RecorgnizesUnsignedIntBellowMinimalBound) {
  const uint32_t value = 499;
  const uint32_t min = 500;
  const uint32_t max = 599;
  EXPECT_FALSE(IsInBounds(value, min, max));
}

TEST(TemplatedBounds, RecorgnizesUnsignedIntAboveMaximalBound) {
  const uint32_t value = 600;
  const uint32_t min = 500;
  const uint32_t max = 599;
  EXPECT_FALSE(IsInBounds(value, min, max));
}

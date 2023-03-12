#include "unit_test_framework.h"
#include

TEST(ExampleSucceedingTest) {
  ASSERT(true);

  ASSERT_TRUE(true);

  ASSERT_FALSE(false);

  ASSERT_EQ(20, 20);

  ASSERT_NULL(NULL);

  int* x = new int;
  ASSERT_NOTNULL(x);
  delete x;

  ASSERT_STREQ("abc", "abc");
}

TEST(ExampleFailingTest) {
  ASSERT_EQ(20, 5);
}

TEST_MAIN();

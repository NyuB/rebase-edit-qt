#include "lib.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <limits>

using testing::Eq;
using testing::Values;

TEST(HelloTest, SimpleAssert) { ASSERT_EQ(7 * 7, 49); }

TEST(HelloTest, AssertThat) { ASSERT_THAT(7 * 6, Eq(42)); }

struct AddTestParam {
  int left;
  int right;
  int expected;
};
class AddTest : public testing::TestWithParam<AddTestParam> {};

TEST_P(AddTest, Add) {
  auto param = GetParam();
  ASSERT_EQ(nyub::lib::add(param.left, param.right), param.expected);
}

INSTANTIATE_TEST_SUITE_P(HappyPaths, AddTest,
                         Values(AddTestParam{1, 2, 3}, AddTestParam{2, 1, 3}));
INSTANTIATE_TEST_SUITE_P(ExtremeValues, AddTest,
                         Values(AddTestParam{1, -1, 0},
                                AddTestParam{INT_MAX, 1, INT_MIN}));

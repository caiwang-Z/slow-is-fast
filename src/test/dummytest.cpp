#include <gtest/gtest.h>
#include <tuple>

namespace DummyTest {
int dummyAdd(int x, int y) {
  return x + y;
}

// Use TEST: Simple, dependency-free testing.
TEST(DummyTest, dummyAdd_sucess_9) {
  auto res = dummyAdd(4, 5);
  EXPECT_TRUE(res, 9);
}


// Use`TEST_F`: tests that require shared state or initialization resources.
namespace DummyTestF {
// Define a test fixture class
class MathTest : public ::testing::Test {
  protected:
  void SetUp() override {
    // Initialization before each test
    _a = 5;
    _b = 3;
  }

  void TearDown() override {
    // Clean-up after each test (if needed)
  }

  int _a;
  int _b;
};

// Use the test fixture for testing
TEST_F(MathTest, Addition) {
  EXPECT_EQ(_a + _b, 8);  // Test addition
}

TEST_F(MathTest, Subtraction) {
  EXPECT_EQ(_a - _b, 2);  // Test subtraction
}

}

// Use`TEST_P`:parameterized tests that perform the same test logic on a set of data.
namespace DummyTestP {
// Parameterized test class
class MathParamTest : public ::testing::TestWithParam<std::tuple<int, int, int>> {};

// Parameterized test
TEST_P(MathParamTest, Addition) {
  // Unpack the parameters
  int x        = std::get<0>(GetParam());
  int y        = std::get<1>(GetParam());
  int expected = std::get<2>(GetParam());

  // Test logic
  EXPECT_EQ(x + y, expected);
}

// Register parameterized data
INSTANTIATE_TEST_SUITE_P(MathTests,          // Test suite name
                         MathParamTest,      // Parameterized test class name
                         ::testing::Values(  // List of test parameters
                           std::make_tuple(1, 2, 3),
                           std::make_tuple(5, 3, 8),
                           std::make_tuple(-1, 1, 0)));

}

}



#include <gtest/gtest.h>

#include "Random.h"

constexpr unsigned int ITERATIONS = 100000;
constexpr float EPSILON = 0.05;

/// Tests that RandomDirection only ever returns 1 or -1
TEST(RandomTest, RandomDirCorrectValues) {
    for (unsigned int i = 0; i < ITERATIONS; ++i) {
        const int r = Naito::randomDirection();
        if (r != 1 && r != -1)
            FAIL();
    }
}

/// Tests that RandomDirection has the same chance to return either direction
TEST(RandomTests, RandomDirAverage) {
    float sum = 0;
    for (unsigned int i = 0; i < ITERATIONS; ++i)
        sum += static_cast<float>(Naito::randomDirection());

    sum /= static_cast<float>(ITERATIONS);

    std::cout << "RandomDirAverage = " << sum << std::endl;

    if (sum < -EPSILON || sum > EPSILON)
        FAIL();
}

#include "../src/datastructures/range_maximum_query.hpp"

#include <gtest/gtest.h>

using ::testing::Test;

TEST(RMQTests, paperExample) {
    std::vector<std::size_t> v = {88, 56, 30, 98, 15, 65, 75, 56, 34, 52, 77, 41, 62, 74, 76, 80};

    RangeMaximumQuery<std::size_t> rmq(v);

    ASSERT_EQ(rmq.query(0, 15), 98);
    ASSERT_EQ(rmq.query(0, 2), 56);
    ASSERT_EQ(rmq.query(0, 3), 98);
    ASSERT_EQ(rmq.query(3, 4), 15);
    ASSERT_EQ(rmq.query(14, 15), 80);
    ASSERT_EQ(rmq.query(10, 11), 41);
    ASSERT_EQ(rmq.query(10, 12), 62);
};

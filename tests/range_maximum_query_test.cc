#include <gtest/gtest.h>
#include "../src/datastructures/range_maximum_query.hpp"

using ::testing::Test;

TEST(RMQTests, paperExample) {
    std::vector<std::size_t> v = {88,  56, 30, 98, 15, 65, 75, 56, 34, 52, 77, 41, 62, 74, 76, 80};

    RangeMaximumQuery<std::size_t> rmq(v);

    for (uint i = 0; i < rmq.get_n_levels(); i++) {
        for (std::size_t j = 0; j < rmq.get_level_size(); j++) {
            std::cout << rmq.get_level_value(i, j) << " ";
        }

        std::cout << std::endl;
    }

    // Expected result:
    // 0 56 0 98 0 65 0 56 0 52 0 41 0 74 0 80 
    // 56 0 30 98 65 0 75 75 52 0 77 77 74 0 76 80 
    // 98 98 98 0 15 65 75 75 77 77 41 0 62 74 76 80 
    // 98 98 98 75 75 75 56 0 34 52 77 77 77 77 77 80

    //FAIL();

    ASSERT_EQ(rmq.query(0, 15), 98);
    ASSERT_EQ(rmq.query(0, 2), 56);
    ASSERT_EQ(rmq.query(0, 3), 98);
    ASSERT_EQ(rmq.query(3, 4), 15);
    ASSERT_EQ(rmq.query(14, 15), 80);
    ASSERT_EQ(rmq.query(10, 11), 41);
    ASSERT_EQ(rmq.query(10, 12), 62);
};
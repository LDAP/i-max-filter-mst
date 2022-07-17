#include "../includes/definitions.hpp"
#include "../src/jarnik_prim.h"
#include "../includes/utils.hpp"

#include <algorithm>
#include <gtest/gtest.h>

using ::testing::Test;

TEST(JarnikPrimTests, mst_correctness) {
    algen::WEdgeList edges;
    edges.emplace_back(0, 1, 3);
    edges.emplace_back(0, 2, 1);
    edges.emplace_back(0, 3, 10);
    edges.emplace_back(2, 1, 1);
    edges.emplace_back(1, 3, 1);
    edges.emplace_back(3, 4, 1);
    edges.emplace_back(2, 5, 5);
    algen::add_back_edges(edges);
    JarnikPrim jp;
    auto mst = jp(edges, 6);

    algen::WEdgeList correct_mst;
    correct_mst.emplace_back(0, 2, 1);
    correct_mst.emplace_back(2, 1, 1);
    correct_mst.emplace_back(1, 3, 1);
    correct_mst.emplace_back(3, 4, 1);
    correct_mst.emplace_back(2, 5, 5);
    // auto mst = correct_mst;
    for (std::size_t i = 0; i < 5; ++i) {
        ASSERT_EQ(mst[i].tail, correct_mst[i].tail);
        ASSERT_EQ(mst[i].head, correct_mst[i].head);
        ASSERT_EQ(mst[i].weight, correct_mst[i].weight);
    }
};

TEST(JarnikPrimTests, msf_correctness) {
    algen::WEdgeList edges;
    edges.emplace_back(0, 1, 3);
    edges.emplace_back(0, 2, 1);
    edges.emplace_back(0, 3, 10);
    edges.emplace_back(2, 1, 1);
    edges.emplace_back(1, 3, 1);
    edges.emplace_back(3, 4, 1);
    edges.emplace_back(2, 5, 5);

    edges.emplace_back(9, 10, 1);
    edges.emplace_back(9, 11, 3);
    edges.emplace_back(10, 12, 2);

    edges.emplace_back(42, 43, 1);

    algen::add_back_edges(edges);
    JarnikPrim jp;
    auto mst = jp(edges, 11);

    algen::WEdgeList correct_mst;
    correct_mst.emplace_back(0, 2, 1);
    correct_mst.emplace_back(2, 1, 1);
    correct_mst.emplace_back(1, 3, 1);
    correct_mst.emplace_back(3, 4, 1);
    correct_mst.emplace_back(2, 5, 5);
    correct_mst.emplace_back(9, 10, 1);
    correct_mst.emplace_back(10, 12, 2);
    correct_mst.emplace_back(9, 11, 3);
    correct_mst.emplace_back(42, 43, 1);
    for (std::size_t i = 0; i < 9; ++i) {
        ASSERT_EQ(mst[i].tail, correct_mst[i].tail);
        ASSERT_EQ(mst[i].head, correct_mst[i].head);
        ASSERT_EQ(mst[i].weight, correct_mst[i].weight);
    }
};

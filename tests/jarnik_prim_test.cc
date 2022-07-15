#include <algorithm>
#include <gtest/gtest.h>
#include "../src/jarnik_prim.h"
#include "../includes/definitions.hpp"

using ::testing::Test;

TEST(JarnikPrimTests, correctness) {
  algen::WEdgeList edges;
  edges.emplace_back(0, 1, 3);
  edges.emplace_back(0, 2, 1);
  edges.emplace_back(0, 3, 10);
  edges.emplace_back(2, 1, 1);
  edges.emplace_back(1, 3, 1);
  edges.emplace_back(3, 4, 1);
  edges.emplace_back(2, 5, 5);
  std::random_shuffle(edges.begin(), edges.end());
  JarnikPrim jp;
  auto mst = jp(edges, 6);

  algen::WEdgeList correct_mst;
  correct_mst.emplace_back(0, 0, 0);
  correct_mst.emplace_back(0, 2, 1);
  correct_mst.emplace_back(2, 1, 1);
  correct_mst.emplace_back(1, 3, 1);
  correct_mst.emplace_back(3, 4, 1);
  correct_mst.emplace_back(2, 5, 5);
  // auto mst = correct_mst;
  for (std::size_t i = 0; i < 6; ++i) {
    ASSERT_EQ(mst[i].tail, correct_mst[i].tail);
    ASSERT_EQ(mst[i].head, correct_mst[i].head);
    ASSERT_EQ(mst[i].weight, correct_mst[i].weight);
  }
};

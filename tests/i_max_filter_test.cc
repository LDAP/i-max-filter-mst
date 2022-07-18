#include "../benchmark/graph_generation.hpp"
#include "../includes/definitions.hpp"
#include "../includes/utils.hpp"
#include "../src/i_max_filter.hpp"
#include "../src/jarnik_prim.h"

#include <gtest/gtest.h>

using ::testing::Test;

TEST(IMaxFilterTests, correctness) {
    benchmark::GNM_Generator generator;
    const std::size_t max_edge_weight = 255;
    std::size_t log_n = 10;
    std::size_t log_m = 10;
    generator.configure(log_n, log_m, max_edge_weight);
    auto gen_edges = generator.generate();

    const std::size_t num_vertices = 1ull << log_n;
    const auto mst_org = fast_kruskal(gen_edges, 1ull << log_n);
    const auto mst_i_max_filter = IMaxFilter()(gen_edges, 1ull << log_n);
    ASSERT_EQ(algen::sum_weights(mst_org), algen::sum_weights(mst_i_max_filter));
};

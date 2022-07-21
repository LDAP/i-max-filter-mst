#include "../benchmark/graph_generation.hpp"
#include "../includes/definitions.hpp"
#include "../includes/utils.hpp"
#include "../src/jarnik_prim.h"

#include <algorithm>
#include <gtest/gtest.h>

using ::testing::Test;

TEST(JarnikPrimTests, mst_correctness) {
    const algen::VertexId num_vertices = 6;
    algen::WEdgeList edges;
    edges.emplace_back(0, 1, 3);
    edges.emplace_back(0, 2, 1);
    edges.emplace_back(0, 3, 10);
    edges.emplace_back(2, 1, 1);
    edges.emplace_back(1, 3, 1);
    edges.emplace_back(3, 4, 1);
    edges.emplace_back(2, 5, 5);
    JarnikPrim jp(num_vertices);
    algen::WEdgeList mst;
    AdjacencyArray adj_arr;
    adj_arr.constructFromUndirected(edges, num_vertices);
    jp.jarnik_prim(adj_arr, mst);

    algen::WEdgeList correct_mst;
    correct_mst.emplace_back(0, 2, 1);
    correct_mst.emplace_back(2, 0, 1);
    correct_mst.emplace_back(2, 1, 1);
    correct_mst.emplace_back(1, 2, 1);
    correct_mst.emplace_back(1, 3, 1);
    correct_mst.emplace_back(3, 1, 1);
    correct_mst.emplace_back(3, 4, 1);
    correct_mst.emplace_back(4, 3, 1);
    correct_mst.emplace_back(2, 5, 5);
    correct_mst.emplace_back(5, 2, 5);
    // auto mst = correct_mst;
    for (std::size_t i = 0; i < 10; ++i) {
        ASSERT_EQ(mst[i].tail, correct_mst[i].tail);
        ASSERT_EQ(mst[i].head, correct_mst[i].head);
        ASSERT_EQ(mst[i].weight, correct_mst[i].weight);
    }
};

TEST(JarnikPrimTests, msf_correctness) {
    const algen::VertexId num_vertices = 44;
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

    JarnikPrim jp(num_vertices);
    algen::WEdgeList mst;
    AdjacencyArray adj_arr;
    adj_arr.constructFromUndirected(edges, num_vertices);
    jp.jarnik_prim(adj_arr, mst);

    algen::WEdgeList correct_mst;
    correct_mst.emplace_back(0, 2, 1);
    correct_mst.emplace_back(2, 0, 1);
    correct_mst.emplace_back(2, 1, 1);
    correct_mst.emplace_back(1, 2, 1);
    correct_mst.emplace_back(1, 3, 1);
    correct_mst.emplace_back(3, 1, 1);
    correct_mst.emplace_back(3, 4, 1);
    correct_mst.emplace_back(4, 3, 1);
    correct_mst.emplace_back(2, 5, 5);
    correct_mst.emplace_back(5, 2, 5);
    correct_mst.emplace_back(9, 10, 1);
    correct_mst.emplace_back(10, 9, 1);
    correct_mst.emplace_back(10, 12, 2);
    correct_mst.emplace_back(12, 10, 2);
    correct_mst.emplace_back(9, 11, 3);
    correct_mst.emplace_back(11, 9, 3);
    correct_mst.emplace_back(42, 43, 1);
    correct_mst.emplace_back(43, 42, 1);
    for (std::size_t i = 0; i < 18; ++i) {
        ASSERT_EQ(mst[i].tail, correct_mst[i].tail);
        ASSERT_EQ(mst[i].head, correct_mst[i].head);
        ASSERT_EQ(mst[i].weight, correct_mst[i].weight);
    }
};

TEST(JarnikPrimTests, generator_correctness) {
    benchmark::GNM_Generator generator;
    const std::size_t max_edge_weight = 255;
    std::size_t log_n = 10;
    std::size_t log_m = 10;
    generator.configure(log_n, log_m, max_edge_weight);
    auto gen_edges = generator.generate();

    const std::size_t num_vertices = 1ull << log_n;
    const auto mst_org = fast_kruskal(gen_edges, 1ull << log_n);
    algen::WEdgeList mst_jp;
    AdjacencyArray adj_arr;
    adj_arr.constructFromDirected(gen_edges, num_vertices);
    JarnikPrim(num_vertices).jarnik_prim(adj_arr, mst_jp);
    ASSERT_EQ(algen::sum_weights(mst_org), algen::sum_weights(mst_jp));
};

TEST(JarnikPrimTests, time) {
    benchmark::GNM_Generator generator;
    const std::size_t max_edge_weight = 255;
    std::size_t log_n = 12;
    std::size_t log_m = 20;
    generator.configure(log_n, log_m, max_edge_weight);
    auto gen_edges = generator.generate();

    const std::size_t num_vertices = 1ull << log_n;
    algen::WEdgeList mst_jp;
    AdjacencyArray adj_arr;
    adj_arr.constructFromUndirected(gen_edges, num_vertices);
    JarnikPrim(num_vertices).jarnik_prim(adj_arr, mst_jp);

    FAIL();
};

TEST(IMaxJarnikPrimTests, mst_correctness) {
    const algen::VertexId num_vertices = 6;
    algen::WEdgeList edges;
    edges.emplace_back(0, 1, 3);
    edges.emplace_back(0, 2, 1);
    edges.emplace_back(0, 3, 10);
    edges.emplace_back(2, 1, 1);
    edges.emplace_back(1, 3, 1);
    edges.emplace_back(3, 4, 1);
    edges.emplace_back(2, 5, 5);
    JarnikPrim jp(num_vertices);
    algen::WEdgeList mst;
    std::vector<std::size_t> jp_nums(num_vertices, -1);
    std::vector<algen::Weight> jp_weights;
    AdjacencyArray adj_arr;
    adj_arr.constructFromUndirected(edges, num_vertices);
    jp.i_max_filter_jarnik_prim(adj_arr, mst, jp_nums, jp_weights);

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

TEST(IMaxJarnikPrimTests, msf_correctness) {
    const algen::VertexId num_vertices = 44;
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

    JarnikPrim jp(num_vertices);
    algen::WEdgeList mst;
    std::vector<std::size_t> jp_nums(num_vertices, -1);
    std::vector<algen::Weight> jp_weights;
    AdjacencyArray adj_arr;
    adj_arr.constructFromUndirected(edges, num_vertices);
    jp.i_max_filter_jarnik_prim(adj_arr, mst, jp_nums, jp_weights);

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

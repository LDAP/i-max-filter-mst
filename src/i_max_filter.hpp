#pragma once

#include "../includes/definitions.hpp"
#include "datastructures/range_maximum_query.hpp"
#include "jarnik_prim.h"

#include <chrono>

class IMaxFilter {
  public:
    algen::WEdgeList operator()(const algen::WEdgeList &edge_list, const algen::VertexId num_vertices) {
        const std::size_t sample_size = std::sqrt(edge_list.size() / 2. * num_vertices);
        algen::WEdgeList edges;
        edges.reserve(sample_size * 1.05);
        std::vector<algen::Weight> jp_weights;
        std::vector<std::size_t> jp_nums(num_vertices);
        JarnikPrim jp(num_vertices);

        // MST from sampled edges
        sample_edges(edge_list, edges, sample_size);
        std::cout << sample_size << " " << edges.size() << "\n";

        AdjacencyArray adj_arr;
        adj_arr.constructFromUndirected(edges, num_vertices);
        // HACK: we reuse the edges array
        edges.clear();
        jp.i_max_filter_jarnik_prim(adj_arr, edges, jp_nums, jp_weights);

        // RMQ setup
        auto rmq_buildup_begin = std::chrono::high_resolution_clock::now();
        RangeMaximumQuery<algen::Weight> rmq(jp_weights);
        auto rmq_buildup_end = std::chrono::high_resolution_clock::now();
        std::cout << "RMQ Buildup: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(rmq_buildup_end - rmq_buildup_begin).count() / 1000.
                  << "\n";

        // Filter loop
        auto filter_begin = std::chrono::high_resolution_clock::now();
        edges.reserve(edges.size() + sample_size);
        std::copy_if(begin(edge_list), end(edge_list), std::back_inserter(edges), [&rmq, &jp_nums](const auto &e) {
            return (e.tail < e.head) && e.weight < rmq.query(jp_nums[e.tail], jp_nums[e.head]);
        });
        auto filter_end = std::chrono::high_resolution_clock::now();
        std::cout << "Filter: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(filter_end - filter_begin).count() / 1000.
                  << "\n";

        // Final MST
        algen::WEdgeList final_mst;
        AdjacencyArray final_adj_arr;
        final_adj_arr.constructFromUndirected(edges, num_vertices);
        jp.jarnik_prim(final_adj_arr, final_mst);
        return final_mst;
    }

    void sample_edges(const algen::WEdgeList &edges,
                      algen::WEdgeList &out,
                      const std::size_t expected_sample_size) {
        auto sample_begin = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < edges.size(); i += edges.size() / expected_sample_size) {
            out.emplace_back(edges[i]);
        }
        auto sample_end = std::chrono::high_resolution_clock::now();
        std::cout << "Sample Edges: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(sample_end - sample_begin).count() / 1000.
                  << "\n";
    }
};

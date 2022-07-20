#pragma once

#include "../includes/definitions.hpp"
#include "datastructures/range_maximum_query.hpp"
#include "jarnik_prim.h"
#include <bits/chrono.h>
#include <chrono>

#include <random>

class IMaxFilter {
  public:
    algen::WEdgeList operator()(const algen::WEdgeList &edge_list, const algen::VertexId num_vertices) {
        const std::size_t seed = 42;
        const std::size_t sample_size = std::sqrt(edge_list.size() / 2. * num_vertices);
        algen::WEdgeList edge_sample;
        edge_sample.reserve(sample_size * 1.05);
        algen::WEdgeList msf;
        std::vector<algen::Weight> jp_weights;
        std::vector<std::size_t> jp_nums(num_vertices);
        JarnikPrim jp(num_vertices);

        // MST from sampled edges
        sample_edges(edge_list, edge_sample, sample_size, seed);
        jp.i_max_filter_jarnik_prim(edge_sample, msf, jp_nums, jp_weights);

        // RMQ setup
        auto rmq_buildup_begin = std::chrono::high_resolution_clock::now();
        RangeMaximumQuery<algen::Weight> rmq(jp_weights);
        auto rmq_buildup_end = std::chrono::high_resolution_clock::now();
        std::cout << "RMQ Buildup: " << std::chrono::duration_cast<std::chrono::microseconds>(rmq_buildup_end - rmq_buildup_begin).count()/1000. << "\n";

        // Filter loop
        auto filter_begin = std::chrono::high_resolution_clock::now();
        std::copy_if(begin(edge_list), end(edge_list), std::back_inserter(msf), [&rmq, &jp_nums, &jp](const auto &e) {
            return jp.get_component_id(e.tail) != jp.get_component_id(e.head) ||
                   e.weight < rmq.query(jp_nums[e.tail], jp_nums[e.head]);
        });
        auto filter_end = std::chrono::high_resolution_clock::now();
        std::cout << "Filter: " << std::chrono::duration_cast<std::chrono::microseconds>(filter_end - filter_begin).count()/1000. << "\n";

        // Final MST
        algen::WEdgeList final_mst;
        jp.jarnik_prim(msf, final_mst);
        return final_mst;
    }

    void sample_edges(const algen::WEdgeList &edges,
                      algen::WEdgeList &out,
                      const std::size_t expected_sample_size,
                      const std::size_t seed) {
        std::mt19937 rng(seed);
        std::uniform_real_distribution<double> unif(0, 1);

        std::size_t n_edges = edges.size() / 2;
        double p = expected_sample_size / (double)n_edges;
        auto sample_begin = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < edges.size(); i++) {
            if (edges[i].head < edges[i].tail && unif(rng) < p) {
                out.emplace_back(edges[i].head, edges[i].tail, edges[i].weight);
                out.emplace_back(edges[i].tail, edges[i].head, edges[i].weight);
            }
        }
        auto sample_end = std::chrono::high_resolution_clock::now();
        std::cout << "Sample Edges: " << std::chrono::duration_cast<std::chrono::microseconds>(sample_end - sample_begin).count()/1000. << "\n";
    }
};

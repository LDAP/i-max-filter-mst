#pragma once

#include "../includes/definitions.hpp"
#include "datastructures/range_maximum_query.hpp"
#include "jarnik_prim.h"

#include <random>

class IMaxFilter {
  public:
    algen::WEdgeList operator()(const algen::WEdgeList &edge_list, const algen::VertexId num_vertices) {
        const std::size_t seed = 42;
        const std::size_t sample_size = std::sqrt(edge_list.size() / 2. * num_vertices);
        algen::WEdgeList edge_sample;
        edge_sample.reserve(sample_size);
        algen::WEdgeList msf;
        std::vector<algen::Weight> jp_weights;
        std::vector<std::size_t> component_ids(num_vertices, -1);
        std::vector<std::size_t> jp_nums(num_vertices, 0);
        JarnikPrim jp;

        // MST from sampled edges
        sample_edges(edge_list, edge_sample, sample_size, seed);
        jp(edge_sample, msf, num_vertices, component_ids, jp_nums, jp_weights);

        // RMQ setup
        RangeMaximumQuery<algen::Weight> rmq(jp_weights);

        // Filter loop
        std::copy_if(begin(edge_list),
                     end(edge_list),
                     std::back_inserter(msf),
                     [&rmq, &jp_nums, &component_ids](const auto &e) {
                         return component_ids[e.tail] != component_ids[e.head] ||
                                e.weight < rmq.query(jp_nums[e.tail], jp_nums[e.head]);
                     });

        // Final MST
        algen::WEdgeList final_mst;
        jp(msf, final_mst, num_vertices, component_ids, jp_nums, jp_weights);
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
        for (std::size_t i = 0; i < edges.size(); i++) {
            if (edges[i].head < edges[i].tail && unif(rng) < p) {
                out.push_back({edges[i].head, edges[i].tail, edges[i].weight});
                out.push_back({edges[i].tail, edges[i].head, edges[i].weight});
            }
        }
    }
};

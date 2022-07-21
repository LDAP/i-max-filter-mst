#pragma once

#include "../includes/definitions.hpp"
#include "datastructures/range_maximum_query.hpp"
#include "jarnik_prim.h"

class IMaxFilter {
  public:
    algen::WEdgeList operator()(const algen::WEdgeList &edge_list, const algen::VertexId num_vertices) {
        const std::size_t sample_size = std::sqrt(edge_list.size() / 2. * num_vertices);

        // Compare expected runtimes of single Jarnik-Prim with I-Max-Filter
        // and use single Jarnik-Prim in case of low average degree.
        const float T_FILTER = .7;
        if (edge_list.size() < (2 * sample_size) + edge_list.size() * T_FILTER) {
            return JarnikPrim(num_vertices)(edge_list, num_vertices);
        }

        algen::WEdgeList edges;
        edges.reserve(sample_size * 1.05);
        std::vector<algen::Weight> jp_weights;
        std::vector<std::size_t> jp_nums(num_vertices);
        JarnikPrim jp(num_vertices);

        // MST from sampled edges
        sample_edges(edge_list, edges, sample_size);

        AdjacencyArray adj_arr;
        adj_arr.constructFromUndirected(edges, num_vertices);
        // HACK: we reuse the edges array
        edges.clear();
        jp.i_max_filter_jarnik_prim(adj_arr, edges, jp_nums, jp_weights);

        // RMQ setup
        RangeMaximumQuery<algen::Weight> rmq(jp_weights);

        // Filter loop
        edges.reserve(edges.size() + sample_size);
        std::copy_if(begin(edge_list), end(edge_list), std::back_inserter(edges), [&rmq, &jp_nums](const auto &e) {
            return (e.tail < e.head) && (e.weight < rmq.get_level_value(std::__lg(jp_nums[e.tail] ^ jp_nums[e.head]), jp_nums[e.tail]) ||
                                         e.weight < rmq.get_level_value(std::__lg(jp_nums[e.tail] ^ jp_nums[e.head]), jp_nums[e.head]));
        });

        // Final MST
        algen::WEdgeList final_mst;
        AdjacencyArray final_adj_arr;
        final_adj_arr.constructFromUndirected(edges, num_vertices);
        jp.jarnik_prim(final_adj_arr, final_mst);
        return final_mst;
    }

    void sample_edges(const algen::WEdgeList &edges, algen::WEdgeList &out, const std::size_t expected_sample_size) {
        for (std::size_t i = 0; i < edges.size(); i += edges.size() / expected_sample_size) {
            out.emplace_back(edges[i]);
        }
    }
};

#pragma once

#include "../includes/definitions.hpp"
#include "datastructures/range_maximum_query.hpp"
#include "jarnik_prim.h"

#include <random>

class IMaxFilter {
    algen::WEdgeList operator()(const algen::WEdgeList &edge_list, const algen::VertexId num_vertices) {
        const std::size_t inf = std::numeric_limits<std::size_t>::max();
        const std::size_t sample_size = std::sqrt(edge_list.size() * num_vertices);
        algen::WEdgeList edge_sample;
        edge_sample.reserve(sample_size);
        JarnikPrim jp;
        sample_edges(edge_list, edge_sample, sample_size, 42);
        algen::WEdgeList msf = jp(edge_sample, num_vertices);
        std::vector<algen::Weight> rmq_weights(msf.size());
        std::vector<std::size_t> jp_num(num_vertices, inf);
        std::size_t i = 0;
        for (const auto &e : msf) {
            if (jp_num[e.tail] == inf)
                jp_num[e.tail] = i++;
        }
        std::transform(begin(msf), end(msf), begin(rmq_weights), end(rmq_weights), [](const auto &e) { return e.weight; });
        RangeMaximumQuery<algen::Weight> rmq(rmq_weights);
        std::remove_copy_if(begin(edge_list), end(edge_list) std::back_inserter(msf),
                            [](const auto &e) { return e.weight >= rmq.query(jp_num[e.tail], jp_num[e.head]); });
        return msf;
    }

    void sample_edges(const algen::WEdgeList &edges,
                      algen::WEdgeList out,
                      const std::size_t sample_size,
                      const std::size_t seed) {
        std::sample(begin(edges), end(edges), std::back_inserter(out), sample_size, std::mt19937{seed});
    }
};

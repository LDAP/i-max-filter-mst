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
        algen::WEdgeList msf;
        algen::VertexArray isolated_vertices;
        std::vector<std::size_t> component_ids(num_vertices, 0);
        JarnikPrim jp;

        // MST from sampled edges
        sample_edges(edge_list, edge_sample, sample_size, 42);
        jp(edge_sample, msf, num_vertices, component_ids, isolated_vertices);
        add_isolated_vertices(isolated_vertices, edge_list, msf);
        std::vector<std::size_t> jp_num(num_vertices, inf);
        std::size_t i = 0;
        jp_num[msf.front().tail] = i++;
        for (const auto &e : msf) {
            jp_num[e.head] = i++;
        }

        // RMQ setup
        algen::WeightArray rmq_weights(msf.size());
        std::transform(begin(msf), end(msf), begin(rmq_weights), end(rmq_weights), [](const auto &e) { return e.weight; });
        RangeMaximumQuery<algen::Weight> rmq(rmq_weights);

        // Filter loop
        std::remove_copy_if(begin(edge_list),
                            end(edge_list),
                            std::back_inserter(msf),
                            [&rmq, &jp_num, &component_ids](const auto &e) {
                                return component_ids[e.tail] == component_ids[e.head] &&
                                       e.weight >= rmq.query(jp_num[e.tail], jp_num[e.head]);
                            });

        // Final MST
        algen::WEdgeList final_mst;
        jp(msf, final_mst, num_vertices, component_ids, isolated_vertices);
        return final_mst;
    }

    void sample_edges(const algen::WEdgeList &edges,
                      algen::WEdgeList &out,
                      const std::size_t sample_size,
                      const std::size_t seed) {
        std::sample(begin(edges), end(edges), std::back_inserter(out), sample_size, std::mt19937{seed});
    }

    void add_isolated_vertices(const algen::VertexArray &isolated_vertices,
                               const algen::WEdgeList &edges,
                               algen::WEdgeList &msf) {
        std::transform(begin(isolated_vertices), end(isolated_vertices), std::back_inserter(msf), [&edges](const auto &v) {
            return *std::find_if(begin(edges), end(edges), [v](const auto &e) { return e.tail == v; });
        });
    }
};

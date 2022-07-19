#pragma once

#include "datastructures/adjacency_array.hpp"

#include <queue>

class JarnikPrim {

  public:
    using Edge = std::pair<algen::Weight, algen::VertexId>;
    using PriorityQueue = std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>>;
    const algen::Weight W_INF = std::numeric_limits<algen::Weight>::max();
    const std::size_t NO_COMPONENT = -1;

    using GraphRepresentation = AdjacencyArray;

    JarnikPrim(const std::size_t num_vertices) : num_vertices(num_vertices), prev(num_vertices) {}


  private:
    void i_max_filter_jarnik_prim_from_node(const algen::VertexId root,
                                            const JarnikPrim::GraphRepresentation &graph,
                                            std::vector<std::size_t> &jp_nums,
                                            std::vector<algen::Weight> &jp_weights,
                                            algen::WEdgeList &msf);

    void jarnik_prim_from_node(const algen::VertexId root,
                               const JarnikPrim::GraphRepresentation &graph,
                               algen::WEdgeList &msf);

  public:
    void jarnik_prim(const algen::WEdgeList &edge_list, algen::WEdgeList &msf);

    void i_max_filter_jarnik_prim(const algen::WEdgeList &edge_list,
                                  algen::WEdgeList &msf,
                                  std::vector<std::size_t> &jp_nums,
                                  std::vector<algen::Weight> &jp_weights);

    std::size_t get_component_id(algen::VertexId vertexId) {
        return component_ids[vertexId];
    }

  private:
    const std::size_t num_vertices;
    std::vector<algen::VertexId> prev;
    std::vector<algen::Weight> best_weights;
    std::vector<std::size_t> component_ids;
    PriorityQueue pq;
};

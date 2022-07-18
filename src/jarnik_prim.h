#pragma once

#include "datastructures/adjacency_array.hpp"

#include <queue>

class JarnikPrim {

  public:
    using Edge = std::pair<algen::Weight, algen::VertexId>;
    using PriorityQueue = std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>>;
    const algen::Weight W_INF = std::numeric_limits<algen::Weight>::max();

    using GraphRepresentation = AdjacencyArray;

    void operator()(const algen::WEdgeList &edge_list,
                    algen::WEdgeList &msf,
                    const algen::VertexId num_vertices,
                    std::vector<std::size_t> &component_ids);
};

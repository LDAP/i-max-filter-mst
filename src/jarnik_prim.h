#pragma once

#include "datastructures/adjacency_array.hpp"

#include <queue>

class JarnikPrim {

  public:
    using Edge = std::pair<algen::Weight, algen::VertexId>;
    using PriorityQueue = std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>>;
    const algen::Weight W_INF = std::numeric_limits<algen::Weight>::max();

    // TODO: Add different graph representations <noahares>
    using GraphRepresentation = AdjacencyArray;

    algen::WEdgeList operator()(const algen::WEdgeList &edge_list, const algen::VertexId num_vertices);
};

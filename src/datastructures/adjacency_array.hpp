#include "../../includes/definitions.hpp"

#include <algorithm>
#include <iostream>

class AdjacencyArray {
  public:
    using EdgeIterator = std::vector<std::pair<algen::VertexId, algen::Weight>>::const_iterator;

    explicit AdjacencyArray(const algen::WEdgeList &edges, const algen::VertexId num_vertices) {
        _indices.resize(num_vertices + 1);
        _edge_endpoints.resize(edges.size());
        std::vector<bool> vertex_reachable(num_vertices, false);

        std::vector<size_t> edge_count(num_vertices, 0);
        for (const auto &e : edges) {
            edge_count[e.tail]++;
            vertex_reachable[e.head] = true;
            vertex_reachable[e.tail] = true;
        }
        for (size_t i = 1; i <= num_vertices; ++i) {
            _indices[i] = _indices[i - 1] + edge_count[i - 1];
        }
        for (const auto &e : edges) {
            const size_t idx = _indices[e.tail + 1] - edge_count[e.tail]--;
            _edge_endpoints[idx] = std::make_pair(e.head, e.weight);
        }
        _num_reachable_vertices = std::count(vertex_reachable.begin(), vertex_reachable.end(), true);
    }

    EdgeIterator beginEdges(const algen::VertexId v) const {
        return _edge_endpoints.begin() + _indices[v];
    }

    EdgeIterator endEdges(const algen::VertexId v) const {
        return _edge_endpoints.begin() + _indices[v + 1];
    }


    algen::VertexId numReachableVertices() const {
        return _num_reachable_vertices;
    }

  private:
    std::vector<std::size_t> _indices;
    std::vector<std::pair<algen::VertexId, algen::Weight>> _edge_endpoints;
    algen::VertexId _num_reachable_vertices;
};

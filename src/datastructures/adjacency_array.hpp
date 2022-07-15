#include "../../includes/definitions.hpp"
#include <iostream>

class AdjacencyArray {
  public:
    using EdgeIterator = std::vector<algen::VertexId>::const_iterator;

    explicit AdjacencyArray(const algen::WEdgeList &edges, const algen::VertexId num_vertices) {
        _indices.resize(num_vertices + 1);
        _edge_endpoints.resize(edges.size());
        _weights.resize(edges.size());

        std::vector<size_t> edge_count(num_vertices, 0);
        for(const auto &e : edges) {
            edge_count[e.tail]++;
        }
        for(size_t i = 1; i <= num_vertices; ++i) {
            _indices[i] = _indices[i - 1] + edge_count[i - 1];
        }
        for(const auto &e : edges) {
            const size_t idx = _indices[e.tail + 1] - edge_count[e.tail]--;
            _edge_endpoints[idx] = e.head;
            _weights[idx] = e.weight;
        }

        for (const auto i : _indices) {
          std::cout << i << std::endl;
        }
        for (const auto i : _edge_endpoints) {
          std::cout << i << std::endl;
        }

        // TODO: maybe sort by edge weight <noahares>
    }

    EdgeIterator beginEdges(const algen::VertexId v) const {
        return _edge_endpoints.begin() + _indices[v];
    }

    EdgeIterator endEdges(const algen::VertexId v) const {
        return _edge_endpoints.begin() + _indices[v + 1];
    }

    algen::Weight edgeWeight(EdgeIterator it) const {
        return _weights[std::distance(_edge_endpoints.begin(), it)];
    }

  private:
    std::vector<std::size_t> _indices;
    std::vector<algen::VertexId> _edge_endpoints;
    std::vector<algen::Weight> _weights;
};

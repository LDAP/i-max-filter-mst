#include "../../includes/definitions.hpp"
#include "../../includes/utils.hpp"

#include <algorithm>

class AdjacencyArray {
  public:
    using NodeHandle = algen::VertexId;
    using Edge = std::pair<algen::Weight, NodeHandle>;
    using EdgeIterator = std::vector<Edge>::const_iterator;

    explicit AdjacencyArray(const algen::WEdgeList &edges, const algen::VertexId num_vertices)
        : _indices(num_vertices + 1), _edges(edges.size()) {
        std::vector<std::size_t> out_degrees(num_vertices + 1);
        
        for (const algen::WEdge &e : edges) {
            out_degrees[e.tail]++;
        }

        std::size_t sum = 0;
        for (std::size_t i = 0; i < num_vertices + 1; i++) {
            _indices[i] = sum;
            sum += out_degrees[i];
        }

        for (const algen::WEdge &e : edges) {
            _edges[_indices[e.tail] + --out_degrees[e.tail]] = {e.weight, e.head};
        }
    }

    EdgeIterator beginEdges(const algen::VertexId v) const {
        return _edges.begin() + _indices[v];
    }

    EdgeIterator endEdges(const algen::VertexId v) const {
        return _edges.begin() + _indices[v + 1];
    }

  private:
    std::vector<NodeHandle> _indices;
    std::vector<Edge> _edges;
};

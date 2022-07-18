#include "../../includes/definitions.hpp"
#include "../../includes/utils.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_map>

class AdjacencyArray {
  public:
    using EdgeIterator = std::vector<algen::WEdge>::const_iterator;
    using NodeHandle = algen::VertexId;

    explicit AdjacencyArray(const algen::WEdgeList &edges, const algen::VertexId num_vertices) {
        _edges_sorted.resize(edges.size());
        _indices.resize(num_vertices + 1);
        std::partial_sort_copy(begin(edges),
                               end(edges),
                               begin(_edges_sorted),
                               end(_edges_sorted),
                               algen::TailHeadOrder<algen::WEdge>());

        for (algen::VertexId v = 0, idx = 0; v < num_vertices; ++v) {
            _indices[v] = idx;
            while (_edges_sorted[idx].tail == v) {
                idx++;
            }
        }
        _indices[num_vertices] = _edges_sorted.size();
    }

    EdgeIterator beginEdges(const algen::VertexId v) const {
        return _edges_sorted.begin() + _indices[v];
    }

    EdgeIterator endEdges(const algen::VertexId v) const {
        return _edges_sorted.begin() + _indices[v + 1];
    }

  private:
    std::vector<std::size_t> _indices;
    algen::WEdgeList _edges_sorted;
};

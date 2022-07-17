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
        std::partial_sort_copy(begin(edges),
                               end(edges),
                               begin(_edges_sorted),
                               end(_edges_sorted),
                               algen::TailHeadOrder<algen::WEdge>());

        // _indices.reserve(num_vertices >> 1);
        // _vertex_id_to_index.reserve(num_vertices >> 1);
        algen::VertexId current_tail = -1;
        for (std::size_t idx = 0; idx < _edges_sorted.size(); ++idx) {
            if (_edges_sorted[idx].tail != current_tail) {
                current_tail = _edges_sorted[idx].tail;
                _vertex_id_to_index.insert(std::make_pair(current_tail, _indices.size()));
                _indices.push_back(idx);
            }
        }
        _indices.push_back(_edges_sorted.size());
        _num_reachable_vertices = _vertex_id_to_index.size();
    }

    EdgeIterator beginEdges(const algen::VertexId v) const {
        return _edges_sorted.begin() + _indices[vertexIdToIndex(v)];
    }

    EdgeIterator endEdges(const algen::VertexId v) const {
        return _edges_sorted.begin() + _indices[vertexIdToIndex(v) + 1];
    }

    NodeHandle vertexIdToIndex(const algen::VertexId v) const {
        assert(_vertex_id_to_index.find(v) != _vertex_id_to_index.end());
        assert(_vertex_id_to_index.at(v) < _indices.size());
        return _vertex_id_to_index.at(v);
    }

    algen::VertexId numReachableVertices() const {
        return _num_reachable_vertices;
    }

  private:
    std::vector<std::size_t> _indices;
    std::vector<algen::WEdge> _edges_sorted;
    std::unordered_map<algen::VertexId, algen::VertexId> _vertex_id_to_index;
    algen::VertexId _num_reachable_vertices;
};

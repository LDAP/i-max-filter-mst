#include "../../includes/definitions.hpp"
#include "../../includes/utils.hpp"

#include <algorithm>

class AdjacencyArray {
  public:
    using NodeHandle = algen::VertexId;
    using Edge = std::pair<algen::Weight, NodeHandle>;
    using EdgeIterator = std::vector<Edge>::const_iterator;

    /* Expects edges only in one direction */
    void constructFromUndirectedV(const std::vector<algen::WEdgeList> &edges_v, const algen::VertexId num_vertices) {
        std::size_t n_edges = 0;
        for (auto &v : edges_v) {
            n_edges += v.size();
        }

        _indices.resize(num_vertices + 1);
        _edges.resize(2 * n_edges);
        std::vector<std::size_t> out_degrees(num_vertices + 1);

        for (auto &edges : edges_v) {
            for (const algen::WEdge &e : edges) {
                out_degrees[e.tail]++;
                out_degrees[e.head]++;
            }
        }

        std::size_t sum = 0;
        for (std::size_t i = 0; i < num_vertices + 1; i++) {
            _indices[i] = sum;
            sum += out_degrees[i];
        }

        for (auto &edges : edges_v) {
            for (const algen::WEdge &e : edges) {
                _edges[_indices[e.tail] + --out_degrees[e.tail]] = {e.weight, e.head};
                _edges[_indices[e.head] + --out_degrees[e.head]] = {e.weight, e.tail};
            }
        }
    }

    /* Expects edges only in one direction */
    void constructFromUndirected(const algen::WEdgeList &edges, const algen::VertexId num_vertices) {
        _indices.resize(num_vertices + 1);
        _edges.resize(2 * edges.size());
        std::vector<std::size_t> out_degrees(num_vertices + 1);

        for (const algen::WEdge &e : edges) {
            out_degrees[e.tail]++;
            out_degrees[e.head]++;
        }

        std::size_t sum = 0;
        for (std::size_t i = 0; i < num_vertices + 1; i++) {
            _indices[i] = sum;
            sum += out_degrees[i];
        }

        for (const algen::WEdge &e : edges) {
            _edges[_indices[e.tail] + --out_degrees[e.tail]] = {e.weight, e.head};
            _edges[_indices[e.head] + --out_degrees[e.head]] = {e.weight, e.tail};
        }
    }

    /* Expects edges in both directions (edges.size() == 2 * m) */
    void constructFromDirected(const algen::WEdgeList &edges, const algen::VertexId num_vertices) {
        _indices.resize(num_vertices + 1);
        _edges.resize(edges.size());
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

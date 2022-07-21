#pragma once

#include "datastructures/adjacency_array.hpp"
#include "datastructures/pq.hpp"

#include <functional>
#include <queue>

class JarnikPrim {

  public:
    using Edge = std::pair<algen::Weight, algen::VertexId>;

    struct EdgeComparator {
        bool operator()(const Edge &wv1, const Edge &wv2) {
            return wv1.first >= wv2.first;
        }
    };

    struct VertexData {
        algen::VertexId prev;
        algen::Weight best_weight;
        bool visited;
    };

    using PriorityQueue = IndexedPriorityQueue<algen::VertexId, algen::Weight, std::greater<>>;
    const algen::Weight W_INF = std::numeric_limits<algen::Weight>::max();
    const std::size_t NO_COMPONENT = -1;
    const VertexData DEFAULT_VERTEX_DATA = VertexData{(algen::VertexId)-1, W_INF, false};

    using GraphRepresentation = AdjacencyArray;

    JarnikPrim(const std::size_t num_vertices)
        : num_vertices(num_vertices)
        , vertex_data(new VertexData[num_vertices])
        , pq(num_vertices) {}

    ~JarnikPrim() {
        delete[] vertex_data;
    }


  private:
    void i_max_filter_jarnik_prim_from_node(const algen::VertexId root,
                                            const JarnikPrim::GraphRepresentation &graph,
                                            std::vector<std::size_t> &jp_nums,
                                            std::vector<algen::Weight> &jp_weights,
                                            algen::WEdgeList &msf);

    void jarnik_prim_from_node(const algen::VertexId root,
                               const JarnikPrim::GraphRepresentation &graph,
                               algen::WEdgeList &msf);

    void prepare() {
        std::fill(vertex_data, vertex_data + num_vertices, DEFAULT_VERTEX_DATA);
    }

  public:
    void jarnik_prim(const JarnikPrim::GraphRepresentation &graph, algen::WEdgeList &msf);

    void i_max_filter_jarnik_prim(const JarnikPrim::GraphRepresentation &graph,
                                  algen::WEdgeList &msf,
                                  std::vector<std::size_t> &jp_nums,
                                  std::vector<algen::Weight> &jp_weights);

    algen::WEdgeList operator()(const algen::WEdgeList &edge_list, const algen::VertexId) {
        algen::WEdgeList result;
        GraphRepresentation graph;
        graph.constructFromDirected(edge_list, num_vertices);
        jarnik_prim(graph, result);
        return result;
    }

  private:
    const std::size_t num_vertices;
    VertexData *const vertex_data;
    PriorityQueue pq;
};

#pragma once

#include "datastructures/adjacency_array.hpp"

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
        std::size_t component_id;
    };

    using PriorityQueue = std::priority_queue<Edge, std::vector<Edge>, EdgeComparator>;
    const algen::Weight W_INF = std::numeric_limits<algen::Weight>::max();
    const std::size_t NO_COMPONENT = -1;
    const VertexData DEFAULT_VERTEX_DATA = VertexData{(algen::VertexId)-1, W_INF, NO_COMPONENT};

    using GraphRepresentation = AdjacencyArray;

    JarnikPrim(const std::size_t num_vertices) : num_vertices(num_vertices) {
        vertex_data = new VertexData[num_vertices];
    }

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
    void jarnik_prim(const algen::WEdgeList &edge_list, algen::WEdgeList &msf);

    void i_max_filter_jarnik_prim(const algen::WEdgeList &edge_list,
                                  algen::WEdgeList &msf,
                                  std::vector<std::size_t> &jp_nums,
                                  std::vector<algen::Weight> &jp_weights);

    std::size_t get_component_id(algen::VertexId vertexId) {
        return vertex_data[vertexId].component_id;
    }

  private:
    const std::size_t num_vertices;
    VertexData* vertex_data;
    PriorityQueue pq;
};

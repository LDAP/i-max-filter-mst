#include "jarnik_prim.h"

#include <algorithm>
#include <iostream>
#include <utility>

algen::WEdgeList JarnikPrim::operator()(const algen::WEdgeList &edge_list, const algen::VertexId num_vertices) {
    const GraphRepresentation graph(edge_list, num_vertices);
    PriorityQueue pq;
    std::vector<algen::Weight> best_weights(graph.numReachableVertices(), W_INF);
    std::vector<bool> in_msf(graph.numReachableVertices(), false);
    std::vector<algen::VertexId> parents(graph.numReachableVertices(), -1);
    algen::WEdgeList msf;
    algen::VertexId num_msf_edges = graph.numReachableVertices() - 1;
    msf.reserve(num_msf_edges);

    auto root_edge = edge_list.begin();
    while (msf.size() < num_msf_edges) {
        algen::VertexId root = root_edge->tail;
        AdjacencyArray::NodeHandle idx_root = graph.vertexIdToIndex(root);
        best_weights[idx_root] = 0;
        in_msf[idx_root] = true;
        parents[idx_root] = root;
        for (auto it = graph.beginEdges(root); it != graph.endEdges(root); ++it) {
            const algen::VertexId v = it->head;
            const AdjacencyArray::NodeHandle idx_v = graph.vertexIdToIndex(v);
            const algen::Weight w = it->weight;
            best_weights[idx_v] = w;
            parents[idx_v] = root_edge->tail;
            pq.push(std::make_pair(w, v));
        }

        while (!pq.empty() && msf.size() < num_msf_edges) {
            const algen::VertexId u = pq.top().second;
            const AdjacencyArray::NodeHandle idx_u = graph.vertexIdToIndex(u);
            pq.pop();
            if (in_msf[idx_u])
                continue;

            msf.emplace_back(parents[idx_u], u, best_weights[idx_u]);
            in_msf[idx_u] = true;
            for (auto it = graph.beginEdges(u); it != graph.endEdges(u); ++it) {
                const algen::VertexId v = it->head;
                const AdjacencyArray::NodeHandle idx_v = graph.vertexIdToIndex(v);
                const algen::Weight w = it->weight;

                if (!in_msf[idx_v] && best_weights[idx_v] > w) {
                    best_weights[idx_v] = w;
                    parents[idx_v] = u;
                    pq.push(std::make_pair(w, v));
                }
            }
        }

        root_edge = std::find_if(root_edge, edge_list.end(), [&in_msf, &graph](const auto &e) {
            return !in_msf[graph.vertexIdToIndex(e.tail)];
        });
        if (root_edge == edge_list.end())
            break;
        num_msf_edges--;
    }

    // for (const auto& e : msf) {
    //   std::cout << e.tail << "," << e.head << " ";
    // }
    // std::cout << std::endl;

    return msf;
}

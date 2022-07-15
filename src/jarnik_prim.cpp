#include "jarnik_prim.h"

#include <algorithm>
#include <iostream>
#include <utility>

algen::WEdgeList JarnikPrim::operator()(const algen::WEdgeList &edge_list, const algen::VertexId num_vertices) {
    const GraphRepresentation graph(edge_list, num_vertices);
    PriorityQueue pq;
    std::vector<algen::Weight> best_weights(num_vertices, W_INF);
    std::vector<bool> in_msf(num_vertices, false);
    std::vector<algen::VertexId> parents(num_vertices, -1);
    algen::WEdgeList msf;
    algen::VertexId num_msf_edges = graph.numReachableVertices() - 1;
    msf.reserve(num_msf_edges);

    auto root_edge = edge_list.begin();
    while (msf.size() < num_msf_edges) {
        algen::VertexId root = root_edge->tail;
        std::cout << root << std::endl;
        best_weights[root] = 0;
        in_msf[root] = true;
        parents[root] = root;
        for (auto it = graph.beginEdges(root); it != graph.endEdges(root); ++it) {
            const algen::VertexId v = it->first;
            const algen::Weight w = it->second;
            best_weights[v] = w;
            parents[v] = root;
            pq.push(std::make_pair(w, v));
        }

        while (!pq.empty() && msf.size() < num_msf_edges) {
            const algen::VertexId u = pq.top().second;
            pq.pop();
            if (in_msf[u])
                continue;

            msf.emplace_back(parents[u], u, best_weights[u]);
            in_msf[u] = true;
            for (auto it = graph.beginEdges(u); it != graph.endEdges(u); ++it) {
                const algen::VertexId v = it->first;
                const algen::Weight w = it->second;

                if (!in_msf[v] && best_weights[v] > w) {
                    best_weights[v] = w;
                    parents[v] = u;
                    pq.push(std::make_pair(w, v));
                }
            }
        }

        root_edge = std::find_if(edge_list.begin(), edge_list.end(), [&in_msf](const auto &e) { return !in_msf[e.tail]; });
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

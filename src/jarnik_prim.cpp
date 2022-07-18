#include "jarnik_prim.h"

#include <algorithm>
#include <iostream>
#include <utility>

void JarnikPrim::operator()(const algen::WEdgeList &edge_list,
                            algen::WEdgeList &msf,
                            const algen::VertexId num_vertices,
                            std::vector<std::size_t> &component_ids) {
    assert(component_ids.size() == num_vertices);

    const GraphRepresentation graph(edge_list, num_vertices);
    PriorityQueue pq;
    std::vector<algen::Weight> best_weights(num_vertices, W_INF);
    std::vector<bool> in_msf(num_vertices, false);
    std::vector<algen::VertexId> parents(num_vertices, -1);
    algen::VertexId num_msf_edges = num_vertices;
    msf.reserve(num_msf_edges);

    for (algen::VertexId root = 0; root < num_vertices; ++root) {
        if (in_msf[root]) {
            continue;
        } else {
            num_msf_edges--;
        }
        best_weights[root] = 0;
        in_msf[root] = true;
        component_ids[root] = root + 1;
        parents[root] = root;
        for (auto it = graph.beginEdges(root); it != graph.endEdges(root); ++it) {
            const algen::VertexId v = it->head;
            const algen::Weight w = it->weight;
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
            component_ids[u] = root + 1;
            in_msf[u] = true;
            for (auto it = graph.beginEdges(u); it != graph.endEdges(u); ++it) {
                const algen::VertexId v = it->head;
                const algen::Weight w = it->weight;

                if (!in_msf[v] && best_weights[v] > w) {
                    best_weights[v] = w;
                    parents[v] = u;
                    pq.push(std::make_pair(w, v));
                }
            }
        }
    }
    assert(msf.size() == num_msf_edges);
}

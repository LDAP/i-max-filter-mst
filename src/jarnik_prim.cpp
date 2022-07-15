#include "jarnik_prim.h"

#include <utility>
#include <iostream>

algen::WEdgeList JarnikPrim::operator()(const algen::WEdgeList &edge_list, const algen::VertexId num_vertices) {
    const GraphRepresentation graph(edge_list, num_vertices);
    PriorityQueue pq;
    std::vector<algen::Weight> best_weights(num_vertices, W_INF);
    std::vector<bool> in_mst(num_vertices, false);
    std::vector<algen::VertexId> parents(num_vertices, -1);
    algen::WEdgeList mst;
    mst.reserve(num_vertices - 1);

    algen::VertexId root = 0; // TODO: random would be better <noahares>
    pq.push(std::make_pair(0, root));
    best_weights[root] = 0;
    parents[root] = root;

    while(!pq.empty() && mst.size() < num_vertices) {
        const algen::VertexId u = pq.top().second;
        pq.pop();
        if(in_mst[u])
            continue;

        mst.emplace_back(parents[u], u, best_weights[u]);
        in_mst[u] = true;
        for(auto it = graph.beginEdges(u); it != graph.endEdges(u); ++it) {
            const algen::VertexId v = *it;
            const algen::Weight w = graph.edgeWeight(it);

            if(!in_mst[v] && best_weights[v] > w) {
                best_weights[v] = w;
                parents[v] = u;
                pq.push(std::make_pair(w, v));
            }
        }
    }

    for (const auto& e : mst) {
      std::cout << e.tail << "," << e.head << std::endl;
    }

    return mst;
}

#include "jarnik_prim.h"

#include <algorithm>
#include <iostream>
#include <utility>

void jp_from_node(algen::VertexId root,
                  const JarnikPrim::GraphRepresentation &graph,
                  std::vector<algen::Weight> &best_weights,
                  std::vector<algen::VertexId> &prev,
                  std::vector<std::size_t> &component_ids,
                  std::vector<std::size_t> &jp_nums,
                  std::vector<algen::Weight> &jp_weights,
                  algen::WEdgeList &msf) {

    JarnikPrim::PriorityQueue pq;
    const std::size_t no_component = (std::size_t) -1;

    // prepare algorithm by "visiting" root
    best_weights[root] = 0;
    component_ids[root] = root;
    jp_nums[root] = jp_weights.size();
    jp_weights.push_back(0);
    for (auto it = graph.beginEdges(root); it != graph.endEdges(root); ++it) {
        best_weights[it->second] = it->first;
        prev[it->second] = root;
        pq.push(*it);
    }

    while (!pq.empty()) {
        JarnikPrim::GraphRepresentation::Edge current = pq.top(); // weight, vertexid
        pq.pop();

        if (component_ids[current.second] != no_component)
            continue;

        // edge selected
        msf.push_back({prev[current.second], current.second, current.first});
        msf.push_back({current.second, prev[current.second], current.first});

        jp_nums[current.second] = jp_weights.size();
        jp_weights.push_back(current.first);
        component_ids[current.second] = root;

        // check out neighbors of node at other end
        for (auto it = graph.beginEdges(current.second); it != graph.endEdges(current.second); ++it) {
            if (component_ids[it->second] == no_component && it->first < best_weights[it->second]) {
                best_weights[it->second] = it->first;
                prev[it->second] = current.second;
                pq.push(*it);
            }
        }
    }
}

void JarnikPrim::operator()(const algen::WEdgeList &edge_list,
                            algen::WEdgeList &msf,
                            const algen::VertexId num_vertices,
                            std::vector<std::size_t> &component_ids,
                            std::vector<std::size_t> &jp_nums,
                            std::vector<algen::Weight> &jp_weights) {
    assert(jp_weights.size() == 0);

    const GraphRepresentation graph(edge_list, num_vertices);
    std::vector<algen::Weight> best_weights(num_vertices, W_INF);
    std::vector<algen::VertexId> prev(num_vertices, -1);

    jp_nums.resize(num_vertices);
    component_ids.assign(num_vertices, -1);

    msf.reserve(num_vertices);
    jp_weights.reserve(num_vertices);

    for (algen::VertexId root = 0; root < num_vertices; ++root) {
        if (best_weights[root] != W_INF)
            continue;
        jp_from_node(root, graph, best_weights, prev, component_ids, jp_nums, jp_weights, msf);
    }
}

#include "jarnik_prim.h"

#include <algorithm>
#include <iostream>
#include <utility>

void JarnikPrim::i_max_filter_jarnik_prim_from_node(const algen::VertexId root,
                                                    const JarnikPrim::GraphRepresentation &graph,
                                                    std::vector<std::size_t> &jp_nums,
                                                    std::vector<algen::Weight> &jp_weights,
                                                    algen::WEdgeList &msf) {
    assert(pq.empty());

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

        if (component_ids[current.second] != NO_COMPONENT)
            continue;

        // edge selected
        msf.push_back({prev[current.second], current.second, current.first});
        msf.push_back({current.second, prev[current.second], current.first});

        jp_nums[current.second] = jp_weights.size();
        jp_weights.push_back(current.first);
        component_ids[current.second] = root;

        // check out neighbors of node at other end
        for (auto it = graph.beginEdges(current.second); it != graph.endEdges(current.second); ++it) {
            if (component_ids[it->second] == NO_COMPONENT && it->first < best_weights[it->second]) {
                best_weights[it->second] = it->first;
                prev[it->second] = current.second;
                pq.push(*it);
            }
        }
    }
}

void JarnikPrim::i_max_filter_jarnik_prim(const algen::WEdgeList &edge_list,
                                          algen::WEdgeList &msf,
                                          std::vector<std::size_t> &jp_nums,
                                          std::vector<algen::Weight> &jp_weights) {
    assert(msf.size() == 0);
    assert(jp_nums.size() == num_vertices);
    assert(jp_weights.size() == 0);

    const GraphRepresentation graph(edge_list, num_vertices);

    // prepare member variables
    best_weights.assign(num_vertices, W_INF);
    component_ids.assign(num_vertices, NO_COMPONENT);

    msf.reserve(num_vertices);
    jp_weights.reserve(num_vertices);

    for (algen::VertexId root = 0; root < num_vertices; ++root) {
        if (best_weights[root] == W_INF)
            i_max_filter_jarnik_prim_from_node(root, graph, jp_nums, jp_weights, msf);
    }
}

void JarnikPrim::jarnik_prim_from_node(const algen::VertexId root,
                                       const JarnikPrim::GraphRepresentation &graph,
                                       algen::WEdgeList &msf) {
    assert(pq.empty());

    // prepare algorithm by "visiting" root
    best_weights[root] = 0;
    for (auto it = graph.beginEdges(root); it != graph.endEdges(root); ++it) {
        best_weights[it->second] = it->first;
        prev[it->second] = root;
        pq.push(*it);
    }

    while (!pq.empty()) {
        JarnikPrim::GraphRepresentation::Edge current = pq.top(); // weight, vertexid
        pq.pop();

        if (component_ids[current.second] != NO_COMPONENT)
            continue;

        // edge selected
        msf.push_back({prev[current.second], current.second, current.first});
        msf.push_back({current.second, prev[current.second], current.first});

        component_ids[current.second] = root;

        // check out neighbors of node at other end
        for (auto it = graph.beginEdges(current.second); it != graph.endEdges(current.second); ++it) {
            if (component_ids[it->second] == NO_COMPONENT && it->first < best_weights[it->second]) {
                best_weights[it->second] = it->first;
                prev[it->second] = current.second;
                pq.push(*it);
            }
        }
    }
}

void JarnikPrim::jarnik_prim(const algen::WEdgeList &edge_list, algen::WEdgeList &msf) {
    assert(msf.size() == 0);

    const GraphRepresentation graph(edge_list, num_vertices);

    // prepare member variables
    best_weights.assign(num_vertices, W_INF);
    component_ids.assign(num_vertices, NO_COMPONENT);

    msf.reserve(num_vertices);

    for (algen::VertexId root = 0; root < num_vertices; ++root) {
        if (best_weights[root] == W_INF)
            jarnik_prim_from_node(root, graph, msf);
    }
}

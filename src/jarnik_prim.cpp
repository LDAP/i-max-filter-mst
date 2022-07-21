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
    vertex_data[root].best_weight = 0;
    vertex_data[root].visited = true;
    jp_nums[root] = jp_weights.size();
    jp_weights.push_back(W_INF);
    for (auto it = graph.beginEdges(root); it != graph.endEdges(root); ++it) {
        vertex_data[it->second].best_weight = it->first;
        vertex_data[it->second].prev = root;
        pq.pushOrChangePriority(it->second, it->first);
    }

    while (!pq.empty()) {
        auto [u, w] = pq.pop(); // weight, vertexid

        if (vertex_data[u].visited)
            continue;
        vertex_data[u].visited = true;

        // edge selected
        msf.emplace_back(vertex_data[u].prev, u, w);

        jp_nums[u] = jp_weights.size();
        jp_weights.push_back(w);

        if (msf.size() == num_vertices - 1) {
            return;
        }

        // check out neighbors of node at other end
        for (auto it = graph.beginEdges(u); it != graph.endEdges(u); ++it) {
            if (!vertex_data[it->second].visited && it->first < vertex_data[it->second].best_weight) {
                vertex_data[it->second].best_weight = it->first;
                vertex_data[it->second].prev = u;
                pq.pushOrChangePriority(it->second, it->first);
            }
        }
    }
}

void JarnikPrim::i_max_filter_jarnik_prim(const JarnikPrim::GraphRepresentation &graph,
                                          algen::WEdgeList &msf,
                                          std::vector<std::size_t> &jp_nums,
                                          std::vector<algen::Weight> &jp_weights) {
    assert(msf.size() == 0);
    assert(jp_nums.size() == num_vertices);
    assert(jp_weights.size() == 0);
    assert(std::all_of(vertex_data, vertex_data + num_vertices, [&](VertexData const &e) {
        return e.best_weight == DEFAULT_VERTEX_DATA.best_weight && e.visited == DEFAULT_VERTEX_DATA.visited &&
               e.prev == DEFAULT_VERTEX_DATA.prev;
    }));

    auto sample_mst_begin = std::chrono::high_resolution_clock::now();
    prepare();

    msf.reserve(num_vertices);
    jp_weights.reserve(num_vertices);

    for (algen::VertexId root = 0; root < num_vertices; ++root) {
        if (vertex_data[root].best_weight == W_INF)
            i_max_filter_jarnik_prim_from_node(root, graph, jp_nums, jp_weights, msf);
    }
    auto sample_mst_end = std::chrono::high_resolution_clock::now();
    std::cout << "Sample MST: "
              << std::chrono::duration_cast<std::chrono::microseconds>(sample_mst_end - sample_mst_begin).count() / 1000.
              << "\n";
}

void JarnikPrim::jarnik_prim_from_node(const algen::VertexId root,
                                       const JarnikPrim::GraphRepresentation &graph,
                                       algen::WEdgeList &msf) {
    assert(pq.empty());

    // prepare algorithm by "visiting" root
    vertex_data[root].best_weight = 0;
    for (auto it = graph.beginEdges(root); it != graph.endEdges(root); ++it) {
        vertex_data[it->second].best_weight = it->first;
        vertex_data[it->second].prev = root;
        pq.pushOrChangePriority(it->second, it->first);
    }

    while (!pq.empty()) {
        auto [u, w] = pq.pop(); // weight, vertexid

        if (vertex_data[u].visited)
            continue;
        vertex_data[u].visited = true;

        // edge selected
        msf.emplace_back(vertex_data[u].prev, u, w);
        msf.emplace_back(u, vertex_data[u].prev, w);

        if (msf.size() >> 1 == num_vertices - 1) {
            return;
        }

        // check out neighbors of node at other end
        for (auto it = graph.beginEdges(u); it != graph.endEdges(u); ++it) {
            if (!vertex_data[it->second].visited && it->first < vertex_data[it->second].best_weight) {
                vertex_data[it->second].best_weight = it->first;
                vertex_data[it->second].prev = u;
                pq.pushOrChangePriority(it->second, it->first);
            }
        }
    }
}

void JarnikPrim::jarnik_prim(const JarnikPrim::GraphRepresentation &graph, algen::WEdgeList &msf) {
    assert(msf.size() == 0);
    assert(std::all_of(vertex_data, vertex_data + num_vertices, [&](VertexData const &e) {
        return e.best_weight == DEFAULT_VERTEX_DATA.best_weight && e.visited == DEFAULT_VERTEX_DATA.visited &&
               e.prev == DEFAULT_VERTEX_DATA.prev;
    }));

    auto mst_begin = std::chrono::high_resolution_clock::now();
    prepare();

    msf.reserve(num_vertices);

    for (algen::VertexId root = 0; root < num_vertices; ++root) {
        if (vertex_data[root].best_weight == W_INF)
            jarnik_prim_from_node(root, graph, msf);
    }
    auto mst_end = std::chrono::high_resolution_clock::now();
    std::cout << "JP MST: " << std::chrono::duration_cast<std::chrono::microseconds>(mst_end - mst_begin).count() / 1000.
              << std::endl;
}

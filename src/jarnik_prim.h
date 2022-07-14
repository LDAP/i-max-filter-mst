#pragma once

#include "includes/definitions.hpp"

class JarnikPrim {

    algen::WEdgeList operator()(const algen::WEdgeList &edge_list, const algen::VertexId num_vertices);
};

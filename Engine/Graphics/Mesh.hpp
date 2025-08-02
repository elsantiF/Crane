#pragma once

#include "Vertex.hpp"

namespace Crane::Graphics {
  struct Mesh {
    SVertex2List vertices;
    IndexList indices;
  };
}
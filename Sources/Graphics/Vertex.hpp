#pragma once

#include "Color.hpp"
#include "Math/Vector.hpp"

namespace Crane::Graphics {
  /*
   * Simple 2D Vertex
   */
  struct SVertex2 {
    Math::Vec2f position;
    Color color;
  };

  typedef Vector<SVertex2> SVertex2List;

  typedef Vector<i32> IndexList;
}
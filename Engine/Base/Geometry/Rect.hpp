#pragma once

#include "Base/Types.hpp"
#include "Math/Vector.hpp"

namespace Crane::Geometry {
  struct Rect {
  public:
    Rect(f32 x = 0.0f, f32 y = 0.0f, f32 width = 0.0f, f32 height = 0.0f) : position{x, y}, size{width, height} {}
    Rect(const Math::Vec2f &pos, const Math::Vec2f &size) : position(pos), size(size) {}

  public:
    Math::Vec2f position;
    Math::Vec2f size;
  };
}
#pragma once

#include "Base/Math/Vector.hpp"
#include "Base/Types.hpp"

namespace Crane::Math {
  struct Transform {
    Transform(const Vec2f &position = Vec2f{0.0f, 0.0f}, f32 rotation = 0.0f) : position(position), rotation(rotation) {}

    Vec2f position;
    f32 rotation;
  };
}
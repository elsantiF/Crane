#pragma once

#include "Core/Types.hpp"
#include "Math/Transform.hpp"
#include "Math/Vector.hpp"

namespace Crane::Components {
  struct Transform {
    Math::Transform transform;
    bool dirty = true;

    Transform(Math::Vec2f position = {0.f, 0.f}, f32 rotation = 0.0f) : transform(position, rotation) {}
  };
}
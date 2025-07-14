#pragma once

#include "Core/Types.hpp"
#include <Math/Vector.hpp>

namespace Crane::Components {
  struct Transform {
    Math::Vec2f position;
    f32 rotation;
    bool dirty = false;

    Transform(Math::Vec2f position = {0.f, 0.f}, f32 rotation = 0.0f) : position(position), rotation(rotation) {}
  };
}
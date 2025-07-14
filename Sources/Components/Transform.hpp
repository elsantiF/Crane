#pragma once

#include "Core/Types.hpp"
#include <Math/Vector.hpp>

namespace Crane::Components {
  struct Transform {
    Math::Vec2f position;
    Math::Vec2f scale;
    f32 rotation;
    bool dirty = false;

    Transform(Math::Vec2f position = {0.f, 0.f}, f32 rotation = 0.0f, Math::Vec2f scale = {1.0f, 1.0f})
        : position(position), scale(scale), rotation(rotation) {}
  };
}
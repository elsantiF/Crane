#pragma once

#include "Core/Types.hpp"
#include <Math/Vector.hpp>

namespace Crane::Components {
  struct Transform {
    Math::Vec2f position;
    f32 rotation;
    f32 scaleX, scaleY;
    bool dirty = false;

    Transform(Math::Vec2f position = {0.f, 0.f}, f32 rotation = 0.0f, f32 scaleX = 1.0f, f32 scaleY = 1.0f)
        : position(position), rotation(rotation), scaleX(scaleX), scaleY(scaleY) {}
  };
}
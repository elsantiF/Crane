#pragma once

#include "Core/Types.hpp"

namespace Crane::Components {
  struct TransformComponent {
    f32 x, y;
    f32 rotation;
    f32 scaleX, scaleY;

    TransformComponent(f32 x = 0.0f, f32 y = 0.0f, f32 rotation = 0.0f, f32 scaleX = 1.0f, f32 scaleY = 1.0f)
        : x(x), y(y), rotation(rotation), scaleX(scaleX), scaleY(scaleY) {}
  };
}
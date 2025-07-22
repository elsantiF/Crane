#pragma once

#include "Core/Types.hpp"
#include <box2d/box2d.h>

namespace Crane::Components {
  struct CircleCollider {
    f32 radius;
    b2ShapeId shapeId;
    bool dirty = false;

    CircleCollider(f32 radius, b2ShapeId shapeId) : radius(radius), shapeId(shapeId) {}
  };
}
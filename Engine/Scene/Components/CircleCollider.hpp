#pragma once

#include "Base/Types.hpp"
#include <box2d/box2d.h>

namespace Crane::Scene::Components {
  struct CircleCollider {
    f32 radius;
    b2ShapeId shapeId;
    bool dirty = false;

    CircleCollider(f32 radius, b2ShapeId shapeId) : radius(radius), shapeId(shapeId) {}
  };
}
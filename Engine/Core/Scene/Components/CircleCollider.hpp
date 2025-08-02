#pragma once

#include "Base/Types.hpp"

namespace Crane::Scene::Components {
  struct CircleCollider {
    f32 radius;
    Id shapeId;
    bool dirty = false;

    CircleCollider(f32 radius, Id shapeId) : radius(radius), shapeId(shapeId) {}
  };
}
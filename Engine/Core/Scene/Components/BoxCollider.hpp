#pragma once

#include "Base/Math/Vector.hpp"
#include "Base/Types.hpp"

namespace Crane::Scene::Components {
  struct BoxCollider {
    Math::Vec2f dimensions;
    Id shapeId;
    bool dirty = false;

    BoxCollider(Math::Vec2f dims, Id shapeId) : dimensions(dims), shapeId(shapeId) {}
  };
}
#pragma once

#include "Base/Math/Vector.hpp"
#include <box2d/box2d.h>

namespace Crane::Scene::Components {
  struct BoxCollider {
    Math::Vec2f dimensions;
    b2ShapeId shapeId;
    bool dirty = false;

    BoxCollider(Math::Vec2f dims, b2ShapeId shapeId) : dimensions(dims), shapeId(shapeId) {}
  };
}
#pragma once

#include "Base/Math/Transform.hpp"
#include "Base/Math/Vector.hpp"
#include "Base/Types.hpp"

namespace Crane::Scene::Components {
  struct Transform {
    Math::Transform transform;
    bool dirty = true;

    Transform(Math::Vec3f position = {0.f, 0.f, 0.f}, f32 rotation = 0.0f) : transform(position, rotation) {}
  };
}
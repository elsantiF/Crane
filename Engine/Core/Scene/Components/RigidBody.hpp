#pragma once

#include "Base/Types.hpp"

namespace Crane::Scene::Components {
  struct RigidBody {
    Id bodyId;

    RigidBody(Id id) : bodyId(id) {}
  };
}
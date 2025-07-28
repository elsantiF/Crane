#pragma once

#include <box2d/box2d.h>

namespace Crane::Components {
  struct RigidBody {
    b2BodyId bodyId;

    RigidBody(b2BodyId id) : bodyId(id) {}
  };
}
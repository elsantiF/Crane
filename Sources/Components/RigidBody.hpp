#pragma once

#include <box2d/box2d.h>

namespace Crane::Components {
  struct Rigidbody {
    b2BodyId bodyId;

    Rigidbody(b2BodyId id) : bodyId(id) {}
  };
}
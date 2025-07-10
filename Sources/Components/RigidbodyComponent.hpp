#pragma once

#include <box2d/box2d.h>

namespace Crane::Components {
  struct RigidbodyComponent {
    b2BodyId bodyId;

    RigidbodyComponent(b2BodyId id) : bodyId(id) {}
  };
}
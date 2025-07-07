#pragma once

#include <box2d/box2d.h>

struct RigidbodyComponent {
  b2BodyId bodyId;

  RigidbodyComponent(b2BodyId id) : bodyId(id) {}
};

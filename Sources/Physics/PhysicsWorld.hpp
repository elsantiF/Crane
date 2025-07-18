#pragma once

#include "Core/Types.hpp"
#include <box2d/box2d.h>

namespace Crane::Physics {
  class PhysicsWorld {
  public:
    PhysicsWorld();
    ~PhysicsWorld();

    void Update(f64 deltaTime);

    b2BodyId CreateBody(const b2BodyDef &bodyDef);

  private:
    b2WorldId m_WorldId;
  };
}
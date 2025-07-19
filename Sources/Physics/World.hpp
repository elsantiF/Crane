#pragma once

#include "BodyStructs.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/RigidBody.hpp"
#include "Core/Types.hpp"
#include <box2d/box2d.h>

namespace Crane::Physics {
  class World {
  public:
    World();
    ~World();

    void Update(f64 deltaTime);

    Pair<Components::Rigidbody, Components::BoxCollider> CreateBoxBody(BodyConfig config, f32 ppm);

  private:
    b2WorldId m_WorldId;
  };
}
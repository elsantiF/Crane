#pragma once

#include "BodyStructs.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/CircleCollider.hpp"
#include "Components/RigidBody.hpp"
#include "Core/Types.hpp"
#include <box2d/box2d.h>

namespace Crane::Physics {
  class World {
  public:
    World();
    ~World();

    void Update(f64 deltaTime);

    Pair<Components::RigidBody, Components::BoxCollider> CreateBoxBody(const BoxBodyConfig &config);
    Pair<Components::RigidBody, Components::CircleCollider> CreateCircleBody(const CircleBodyConfig &config);

  private:
    Components::RigidBody CreateRigidBody(const BaseBodyConfig &config);

    b2WorldId m_WorldId;
  };
}
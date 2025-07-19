#pragma once

#include "BodyStructs.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/RigidBody.hpp"
#include "Core/Types.hpp"

namespace Crane::Physics {
  class World;

  class PhysicsFactory {
  public:
    static Pair<Components::Rigidbody, Components::BoxCollider> CreateBoxBody(World &world, BodyConfig config, f32 ppm);
  };
}
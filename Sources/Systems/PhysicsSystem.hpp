#pragma once

#include "Core/Types.hpp"
#include "ISystem.hpp"

namespace Crane::Systems {
  class PhysicsSystem : public IUpdateSystem {
  public:
    PhysicsSystem() = default;

    void Update(World::World &world, f64 deltaTime) override;
  };
}
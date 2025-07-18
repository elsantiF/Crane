#pragma once

#include "Core/Types.hpp"
#include "ISystem.hpp"

namespace Crane::Systems {
  class PhysicsSystem : public IFixedUpdateSystem {
  public:
    PhysicsSystem() = default;

    void Initialize(World::World &world) override {
      (void)world;
    };

    void Shutdown(World::World &world) override {
      (void)world;
    };

    void FixedUpdate(World::World &world, f64 deltaTime) override;
  };
}
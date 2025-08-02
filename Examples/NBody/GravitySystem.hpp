#pragma once

#include "Core/Systems/ISystem.hpp"

class GravitySystem : public Crane::Systems::IFixedUpdateSystem {
public:
  GravitySystem(Crane::Scene::World &world) : Crane::Systems::IFixedUpdateSystem(world) {}
  ~GravitySystem() override = default;

  void FixedUpdate(f64 deltaTime) override;
};
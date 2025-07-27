#pragma once

#include "Components/BoxCollider.hpp"
#include "Components/CircleCollider.hpp"
#include "Components/RigidBody.hpp"
#include "Core/Types.hpp"
#include "ISystem.hpp"
#include "Physics/BodyStructs.hpp"
#include <box2d/box2d.h>

namespace Crane::Systems {
  class PhysicsSystem : public IFixedUpdateSystem {
  public:
    PhysicsSystem(Math::Vec2f gravity) : m_Gravity(gravity) {}

    void Initialize(World::World &world) override;
    void Shutdown(World::World &world) override;

    void FixedUpdate(World::World &world, f64 deltaTime) override;

    Pair<Components::RigidBody, Components::BoxCollider> CreateBoxBody(const Physics::BoxBodyConfig &config);
    Pair<Components::RigidBody, Components::CircleCollider> CreateCircleBody(const Physics::CircleBodyConfig &config);

  private:
    Components::RigidBody CreateRigidBody(const Physics::BaseBodyConfig &config);

  private:
    b2WorldId m_WorldId;
    Math::Vec2f m_Gravity;
  };
}
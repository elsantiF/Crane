#pragma once

#include "Base/Types.hpp"
#include "Core/Systems/ISystem.hpp"
#include "Physics/BodyStructs.hpp"
#include "Scene/Components/BoxCollider.hpp"
#include "Scene/Components/CircleCollider.hpp"
#include "Scene/Components/RigidBody.hpp"

struct b2WorldId;

namespace Crane::Physics {
  class PhysicsSystem : public Core::Systems::IFixedUpdateSystem {
  public:
    PhysicsSystem(Math::Vec2f gravity, f32 pixelsPerMeter, u32 physicsSteps)
        : m_Gravity(gravity), m_PixelsPerMeter(pixelsPerMeter), m_PhysicsSteps(physicsSteps) {}

    void Initialize(Scene::World &world) override;
    void Shutdown(Scene::World &world) override;

    void FixedUpdate(Scene::World &world, f64 deltaTime) override;

    Pair<Scene::Components::RigidBody, Scene::Components::BoxCollider> CreateBoxBody(const Physics::BoxBodyConfig &config);
    Pair<Scene::Components::RigidBody, Scene::Components::CircleCollider> CreateCircleBody(const Physics::CircleBodyConfig &config);

  private:
    Scene::Components::RigidBody CreateRigidBody(const Physics::BaseBodyConfig &config);

  private:
    b2WorldId m_WorldId;
    Math::Vec2f m_Gravity;
    f32 m_PixelsPerMeter;
    u32 m_PhysicsSteps;
  };
}
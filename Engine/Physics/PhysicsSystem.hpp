#pragma once

#include "Base/Types.hpp"
#include "Core/Systems/ISystem.hpp"
#include "Events/Events.hpp"
#include "Physics/BodyStructs.hpp"
#include "Scene/Components/BoxCollider.hpp"
#include "Scene/Components/CircleCollider.hpp"
#include "Scene/Components/RigidBody.hpp"
#include <box2d/box2d.h>

namespace Crane::Physics {
  struct PhysicsSystemConfig {
    Math::Vec2f gravity = {0.0f, -9.81f};
    f32 pixelsPerMeter = 30.0f;
    u32 physicsSteps = 4;
  };

  class PhysicsSystem : public Systems::IFixedUpdateSystem {
  public:
    PhysicsSystem(Scene::World &world, const PhysicsSystemConfig &config);
    ~PhysicsSystem() override;

    void FixedUpdate(f64 deltaTime) override;

    Pair<Scene::Components::RigidBody, Scene::Components::BoxCollider> CreateBoxBody(const Physics::BoxBodyConfig &config);
    Pair<Scene::Components::RigidBody, Scene::Components::CircleCollider> CreateCircleBody(const Physics::CircleBodyConfig &config);

  private:
    Scene::Components::RigidBody CreateRigidBody(const Physics::BaseBodyConfig &config);

    void ApplyForce(const Events::ForceApplied &event);

  private:
    b2WorldId m_WorldId;

    struct {
      UnorderedMap<Id, b2BodyId> bodies;
      UnorderedMap<Id, b2ShapeId> shapes;
    } m_Context;

    PhysicsSystemConfig m_Config;
  };
}
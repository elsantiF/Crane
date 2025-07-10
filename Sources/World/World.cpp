#include "World.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"

namespace Crane {
  World::World() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 9.81f};
    m_WorldId = b2CreateWorld(&worldDef);

    m_Registry.group<Components::Rigidbody, Components::Transform>();
  }

  World::~World() {
    if (b2World_IsValid(m_WorldId)) {
      b2DestroyWorld(m_WorldId);
    }
  }

  void World::Update(f64 deltaTime) {
    b2World_Step(m_WorldId, static_cast<float>(deltaTime), PHYSICS_STEPS);

    auto view = m_Registry.view<Components::Rigidbody, Components::Transform>();
    for (auto [entity, rigidBody, transform] : view.each()) {
      if (!b2Body_IsValid(rigidBody.bodyId)) {
        continue;
      }

      b2Vec2 position = b2Body_GetPosition(rigidBody.bodyId);
      b2Rot angle = b2Body_GetRotation(rigidBody.bodyId);
      f32 angleDegrees = b2Rot_GetAngle(angle);

      transform.x = position.x * PIXELS_PER_METER;
      transform.y = position.y * PIXELS_PER_METER;
      transform.rotation = angleDegrees;
    }
  }
}
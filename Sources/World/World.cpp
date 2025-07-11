#include "World.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Entity.hpp"

namespace Crane::World {
  World::World() : m_PhysicsWorld() { m_Registry.group<Components::Rigidbody, Components::Transform>(); }

  void World::Update(f64 deltaTime) {
    auto dirtyView = m_Registry.view<Components::Rigidbody, Components::Transform>();
    bool anyTransformDirty = false;
    for (auto [entity, rigidBody, transform] : dirtyView.each()) {
      if (!b2Body_IsValid(rigidBody.bodyId) || !transform.dirty) {
        continue;
      }

      b2Vec2 position = {transform.x / PIXELS_PER_METER, transform.y / PIXELS_PER_METER};
      b2Rot angle = b2MakeRot(transform.rotation);
      b2Body_SetTransform(rigidBody.bodyId, position, angle);
      b2Body_SetAwake(rigidBody.bodyId, true);
      transform.dirty = false;
      anyTransformDirty = true;
    }

    if (anyTransformDirty) {
      WakeUpBodies();
    }

    m_PhysicsWorld.Update(deltaTime);

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

  Entity World::CreateEntity() {
    auto entity = m_Registry.create();
    return Entity(this, entity);
  }

  void World::WakeUpBodies() {
    auto view = m_Registry.view<Components::Rigidbody>();
    for (auto [entity, rigidBody] : view.each()) {
      if (b2Body_IsValid(rigidBody.bodyId)) {
        b2Body_SetAwake(rigidBody.bodyId, true);
      }
    }
  }
}
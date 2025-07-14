#include "PhysicsSystem.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "World/World.hpp"

namespace Crane::Systems {
  void PhysicsSystem::Update(World::World &world, f64 deltaTime) {
    auto &registry = world.GetRegistry();
    auto view = registry.view<Components::Rigidbody, Components::Transform>();
    auto ppm = world.GetPixelsPerMeter();
    bool anyTransformDirty = false;

    for (auto [entity, rigidBody, transform] : view.each()) {
      if (!b2Body_IsValid(rigidBody.bodyId) || !transform.dirty) {
        continue;
      }

      b2Vec2 position = {transform.position.x / ppm, transform.position.y / ppm};
      b2Rot angle = b2MakeRot(transform.rotation);
      b2Body_SetTransform(rigidBody.bodyId, position, angle);

      // TODO: This is a temporary fix to the energy accumulation issue.
      // Should find a better way to handle this.
      b2Body_SetLinearVelocity(rigidBody.bodyId, {0.0f, 0.0f});
      b2Body_SetAngularVelocity(rigidBody.bodyId, 0.0f);

      b2Body_SetAwake(rigidBody.bodyId, true);
      transform.dirty = false;
      anyTransformDirty = true;
    }

    if (anyTransformDirty) {
      // Wake up all bodies if any transform was dirty
      auto view = registry.view<Components::Rigidbody>();
      for (auto [entity, rigidBody] : view.each()) {
        if (b2Body_IsValid(rigidBody.bodyId)) {
          b2Body_SetAwake(rigidBody.bodyId, true);
        }
      }
    }

    world.GetPhysicsWorld().Update(deltaTime);

    for (auto [entity, rigidBody, transform] : view.each()) {
      if (!b2Body_IsValid(rigidBody.bodyId)) {
        continue;
      }

      b2Vec2 position = b2Body_GetPosition(rigidBody.bodyId);
      b2Rot angle = b2Body_GetRotation(rigidBody.bodyId);
      f32 angleDegrees = b2Rot_GetAngle(angle);

      transform.position.x = position.x * ppm;
      transform.position.y = position.y * ppm;
      transform.rotation = angleDegrees;
    }
  }
}
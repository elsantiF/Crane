#include "PhysicsSystem.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Core/Profiler.hpp"
#include "World/World.hpp"

namespace Crane::Systems {
  void PhysicsSystem::FixedUpdate(World::World &world, f64 deltaTime) {
    PROFILE_SCOPE();
    auto &registry = world.GetRegistry();
    auto view = registry.view<Components::RigidBody, Components::BoxCollider, Components::Transform>();
    auto ppm = world.GetPixelsPerMeter();
    bool anyTransformDirty = false;

    for (auto [entity, rigidBody, boxCollider, transform] : view.each()) {
      if (!b2Body_IsValid(rigidBody.bodyId) || (!transform.dirty && !boxCollider.dirty)) {
        continue;
      }

      if (transform.dirty) {
        b2Vec2 position = {transform.transform.position.x / ppm, transform.transform.position.y / ppm};
        b2Rot angle = b2MakeRot(transform.transform.rotation);
        b2Body_SetTransform(rigidBody.bodyId, position, angle);
        transform.dirty = false;
      }

      if (boxCollider.dirty) {
        b2ShapeId shapeId = boxCollider.shapeId;
        if (b2Shape_IsValid(shapeId)) {
          b2Polygon boxShape = b2MakeBox(boxCollider.dimensions.x / 2 / ppm, boxCollider.dimensions.y / 2 / ppm);
          b2Shape_SetPolygon(shapeId, &boxShape);
        }
        boxCollider.dirty = false;
      }

      // TODO: This is a temporary fix to the energy accumulation issue.
      // Should find a better way to handle this.
      b2Body_SetLinearVelocity(rigidBody.bodyId, {0.0f, 0.0f});
      b2Body_SetAngularVelocity(rigidBody.bodyId, 0.0f);

      b2Body_SetAwake(rigidBody.bodyId, true);
      anyTransformDirty = true;
    }

    if (anyTransformDirty) {
      // Wake up all bodies if any transform was dirty
      auto view = registry.view<Components::RigidBody>();
      for (auto [entity, rigidBody] : view.each()) {
        if (b2Body_IsValid(rigidBody.bodyId)) {
          b2Body_SetAwake(rigidBody.bodyId, true);
        }
      }
    }

    world.GetPhysicsWorld().Update(deltaTime);

    for (auto [entity, rigidBody, boxCollider, transform] : view.each()) {
      if (!b2Body_IsValid(rigidBody.bodyId)) {
        continue;
      }

      b2Vec2 position = b2Body_GetPosition(rigidBody.bodyId);
      b2Rot angle = b2Body_GetRotation(rigidBody.bodyId);
      f32 angleDegrees = b2Rot_GetAngle(angle);

      transform.transform.position.x = position.x * ppm;
      transform.transform.position.y = position.y * ppm;
      transform.transform.rotation = angleDegrees;
    }
  }
}
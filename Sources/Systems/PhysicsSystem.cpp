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
    auto ppm = world.GetPixelsPerMeter();

    auto rbTransformView = registry.view<Components::RigidBody, Components::Transform>();
    for (auto [entity, rigidBody, transform] : rbTransformView.each()) {
      if (!b2Body_IsValid(rigidBody.bodyId) || !transform.dirty) {
        continue;
      }

      b2Vec2 position = {transform.transform.position.x / ppm, transform.transform.position.y / ppm};
      b2Rot angle = b2MakeRot(transform.transform.rotation);
      b2Body_SetTransform(rigidBody.bodyId, position, angle);
      transform.dirty = false;
    }

    auto dirtyBoxView = registry.view<Components::RigidBody, Components::BoxCollider>();
    for (auto [entity, rigidBody, boxCollider] : dirtyBoxView.each()) {
      if (!b2Body_IsValid(rigidBody.bodyId) || !boxCollider.dirty) {
        continue;
      }

      b2ShapeId shapeId = boxCollider.shapeId;
      if (b2Shape_IsValid(shapeId)) {
        f32 halfWidth = boxCollider.dimensions.x / 2;
        f32 halfHeight = boxCollider.dimensions.y / 2;

        if (halfWidth <= 0.0f) {
          halfWidth = 0.01f;
        }

        if (halfHeight <= 0.0f) {
          halfHeight = 0.01f;
        }

        b2Polygon boxShape = b2MakeBox(halfWidth, halfHeight);
        b2Shape_SetPolygon(shapeId, &boxShape);
      }
      boxCollider.dirty = false;
    }

    auto dirtyCircleView = registry.view<Components::RigidBody, Components::CircleCollider>();
    for (auto [entity, rigidBody, circleCollider] : dirtyCircleView.each()) {
      if (!b2Body_IsValid(rigidBody.bodyId) || !circleCollider.dirty) {
        continue;
      }

      b2ShapeId shapeId = circleCollider.shapeId;
      if (b2Shape_IsValid(shapeId)) {
        f32 radius = circleCollider.radius;
        if (radius <= 0.0f) {
          radius = 0.01f;
        }
        b2Circle circleShape = b2Circle{
            b2Vec2{0.0f, 0.0f},
            radius
        };
        b2Shape_SetCircle(shapeId, &circleShape);
      }
      circleCollider.dirty = false;
    }

    world.GetPhysicsWorld().Update(deltaTime);

    for (auto [entity, rigidBody, transform] : rbTransformView.each()) {
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
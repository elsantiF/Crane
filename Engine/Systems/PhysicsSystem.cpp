#include "PhysicsSystem.hpp"
#include "Base/Logger.hpp"
#include "Base/Profiler.hpp"
#include "Scene/Components/BoxCollider.hpp"
#include "Scene/Components/RigidBody.hpp"
#include "Scene/Components/Transform.hpp"
#include "Scene/World/World.hpp"

namespace Crane::Systems {
  void PhysicsSystem::Initialize(Scene::World &world) {
    PROFILE_SCOPE();
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {m_Gravity.x, m_Gravity.y};
    worldDef.enableSleep = false;
    m_WorldId = b2CreateWorld(&worldDef);
    Logger::Info("PhysicsSystem created");
  }

  void PhysicsSystem::Shutdown(Scene::World &world) {
    if (b2World_IsValid(m_WorldId)) {
      b2DestroyWorld(m_WorldId);
      Logger::Info("PhysicsSystem destroyed");
    } else {
      Logger::Error("PhysicsSystem shutdown called on an invalid world ID");
    }
  }

  void PhysicsSystem::FixedUpdate(Scene::World &world, f64 deltaTime) {
    PROFILE_SCOPE();
    auto &registry = world.GetRegistry();
    auto ppm = world.GetPixelsPerMeter();

    auto rbTransformView = registry.view<Scene::Components::RigidBody, Scene::Components::Transform>();
    for (auto [entity, rigidBody, transform] : rbTransformView.each()) {
      if (!b2Body_IsValid(rigidBody.bodyId) || !transform.dirty) {
        continue;
      }

      b2Vec2 position = {transform.transform.position.x / ppm, transform.transform.position.y / ppm};
      b2Rot angle = b2MakeRot(transform.transform.rotation);
      b2Body_SetTransform(rigidBody.bodyId, position, angle);
      transform.dirty = false;
    }

    auto dirtyBoxView = registry.view<Scene::Components::RigidBody, Scene::Components::BoxCollider>();
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

    auto dirtyCircleView = registry.view<Scene::Components::RigidBody, Scene::Components::CircleCollider>();
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

    b2World_Step(m_WorldId, static_cast<float>(deltaTime), Physics::PHYSICS_STEPS);

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

  Pair<Scene::Components::RigidBody, Scene::Components::BoxCollider> PhysicsSystem::CreateBoxBody(const Physics::BoxBodyConfig &config) {
    PROFILE_SCOPE();
    Scene::Components::RigidBody rigidBody = CreateRigidBody(config);
    f32 ppm = Crane::Scene::PIXELS_PER_METER;
    f32 width = config.dimensions.x / ppm;
    f32 height = config.dimensions.y / ppm;
    b2Polygon boxShape = b2MakeBox(width / 2, height / 2);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    b2ShapeId shapeId = b2CreatePolygonShape(rigidBody.bodyId, &shapeDef, &boxShape);

    return {rigidBody, Scene::Components::BoxCollider(Math::Vec2f{width, height}, shapeId)};
  }

  Pair<Scene::Components::RigidBody, Scene::Components::CircleCollider> PhysicsSystem::CreateCircleBody(const Physics::CircleBodyConfig &config) {
    PROFILE_SCOPE();
    Scene::Components::RigidBody rigidBody = CreateRigidBody(config);
    f32 ppm = Crane::Scene::PIXELS_PER_METER;
    f32 radius = config.radius / ppm;
    b2Circle circleShape = b2Circle{
        {0, 0},
        radius
    };
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    b2ShapeId shapeId = b2CreateCircleShape(rigidBody.bodyId, &shapeDef, &circleShape);

    return {rigidBody, Scene::Components::CircleCollider(radius, shapeId)};
  }

  Scene::Components::RigidBody PhysicsSystem::CreateRigidBody(const Physics::BaseBodyConfig &config) {
    PROFILE_SCOPE();
    f32 ppm = Crane::Scene::PIXELS_PER_METER;
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = static_cast<b2BodyType>(config.type);
    bodyDef.position = {config.center.x / ppm, config.center.y / ppm};
    b2BodyId bodyId = b2CreateBody(m_WorldId, &bodyDef);

    return Scene::Components::RigidBody(bodyId);
  }
}
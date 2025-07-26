#include "World.hpp"
#include "Core/Profiler.hpp"
#include <Core/Config.hpp>

namespace Crane::Physics {
  World::World() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 9.81f};
    worldDef.enableSleep = false;
    m_WorldId = b2CreateWorld(&worldDef);
  }

  World::~World() {
    if (b2World_IsValid(m_WorldId)) {
      b2DestroyWorld(m_WorldId);
    }
  }

  void World::Update(f64 deltaTime) {
    PROFILE_SCOPE();
    b2World_Step(m_WorldId, static_cast<float>(deltaTime), PHYSICS_STEPS);
  }

  Components::RigidBody World::CreateRigidBody(const BaseBodyConfig &config) {
    PROFILE_SCOPE();
    f32 ppm = Crane::World::PIXELS_PER_METER;
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = static_cast<b2BodyType>(config.type);
    bodyDef.position = {config.center.x / ppm, config.center.y / ppm};
    b2BodyId bodyId = b2CreateBody(m_WorldId, &bodyDef);

    return Components::RigidBody(bodyId);
  }

  Pair<Components::RigidBody, Components::BoxCollider> World::CreateBoxBody(const BoxBodyConfig &config) {
    PROFILE_SCOPE();
    Components::RigidBody rigidBody = CreateRigidBody(config);
    f32 ppm = Crane::World::PIXELS_PER_METER;
    f32 width = config.dimensions.x / ppm;
    f32 height = config.dimensions.y / ppm;
    b2Polygon boxShape = b2MakeBox(width / 2, height / 2);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    b2ShapeId shapeId = b2CreatePolygonShape(rigidBody.bodyId, &shapeDef, &boxShape);

    return {rigidBody, Components::BoxCollider(Math::Vec2f{width, height}, shapeId)};
  }

  Pair<Components::RigidBody, Components::CircleCollider> World::CreateCircleBody(const CircleBodyConfig &config) {
    PROFILE_SCOPE();
    Components::RigidBody rigidBody = CreateRigidBody(config);
    f32 ppm = Crane::World::PIXELS_PER_METER;
    f32 radius = config.radius / ppm;
    b2Circle circleShape = b2Circle{
        {0, 0},
        radius
    };
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    b2ShapeId shapeId = b2CreateCircleShape(rigidBody.bodyId, &shapeDef, &circleShape);

    return {rigidBody, Components::CircleCollider(radius, shapeId)};
  }
}
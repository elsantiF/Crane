#include "World.hpp"
#include "Core/Profiler.hpp"
#include <Core/Config.hpp>

namespace Crane::Physics {
  World::World() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 9.81f};
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

  Pair<Components::RigidBody, Components::BoxCollider> World::CreateBoxBody(BoxBodyConfig config, f32 ppm) {
    PROFILE_SCOPE();
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = static_cast<b2BodyType>(config.type);
    bodyDef.position = {config.x / ppm, config.y / ppm};
    b2BodyId bodyId = b2CreateBody(m_WorldId, &bodyDef);

    f32 width = config.width / ppm;
    f32 height = config.height / ppm;
    b2Polygon boxShape = b2MakeBox(width / 2, height / 2);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);

    return MakePair(Components::RigidBody(bodyId), Components::BoxCollider(Math::Vec2f{width, height}, shapeId));
  }

  Pair<Components::RigidBody, Components::CircleCollider> World::CreateCircleBody(CircleBodyConfig config, f32 ppm) {
    PROFILE_SCOPE();
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = static_cast<b2BodyType>(config.type);
    bodyDef.position = {config.x / ppm, config.y / ppm};
    b2BodyId bodyId = b2CreateBody(m_WorldId, &bodyDef);

    f32 radius = config.radius / ppm;
    b2Circle circleShape = b2Circle{
        {0, 0},
        radius
    };
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    b2ShapeId shapeId = b2CreateCircleShape(bodyId, &shapeDef, &circleShape);

    return MakePair(Components::RigidBody(bodyId), Components::CircleCollider(radius, shapeId));
  }
}
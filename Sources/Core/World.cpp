#include "World.hpp"
#include "Components/Rigidbody.hpp"
#include "Components/Transform.hpp"

World::World() {
  b2WorldDef worldDef = b2DefaultWorldDef();
  worldDef.gravity = {0.0f, 9.81f};
  worldId = b2CreateWorld(&worldDef);
}

World::~World() {
  if (worldId.index1 != 0) {
    b2DestroyWorld(worldId);
  }
}

void World::Update(f32 deltaTime) {
  b2World_Step(worldId, deltaTime, PHYSICS_STEPS);

  auto view = registry.view<Rigidbody, Transform>();
  for (auto entity : view) {
    auto &rigidBody = view.get<Rigidbody>(entity);
    auto &transform = view.get<Transform>(entity);

    b2Vec2 position = b2Body_GetPosition(rigidBody.bodyId);
    b2Rot angle = b2Body_GetRotation(rigidBody.bodyId);
    f32 angleDegrees = b2Rot_GetAngle(angle);

    transform.x = position.x * PIXELS_PER_METER;
    transform.y = position.y * PIXELS_PER_METER;
    transform.rotation = angleDegrees;
  }
}
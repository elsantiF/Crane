#include "World.hpp"
#include "Components/RigidbodyComponent.hpp"
#include "Components/TransformComponent.hpp"

World::World() {
  b2WorldDef worldDef = b2DefaultWorldDef();
  worldDef.gravity = {0.0f, 9.81f};
  m_WorldId = b2CreateWorld(&worldDef);
}

World::~World() {
  if (m_WorldId.index1 != 0) {
    b2DestroyWorld(m_WorldId);
  }
}

void World::Update(f64 deltaTime) {
  b2World_Step(m_WorldId, static_cast<float>(deltaTime), PHYSICS_STEPS);

  auto view = m_Registry.view<RigidbodyComponent, TransformComponent>();
  for (auto entity : view) {
    auto &rigidBody = view.get<RigidbodyComponent>(entity);
    auto &transform = view.get<TransformComponent>(entity);

    b2Vec2 position = b2Body_GetPosition(rigidBody.bodyId);
    b2Rot angle = b2Body_GetRotation(rigidBody.bodyId);
    f32 angleDegrees = b2Rot_GetAngle(angle);

    transform.x = position.x * PIXELS_PER_METER;
    transform.y = position.y * PIXELS_PER_METER;
    transform.rotation = angleDegrees;
  }
}
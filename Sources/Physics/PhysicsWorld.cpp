#include "PhysicsWorld.hpp"
#include "Core/Profiler.hpp"
#include <Core/Config.hpp>

namespace Crane::Physics {
  PhysicsWorld::PhysicsWorld() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 9.81f};
    m_WorldId = b2CreateWorld(&worldDef);
  }

  PhysicsWorld::~PhysicsWorld() {
    if (b2World_IsValid(m_WorldId)) {
      b2DestroyWorld(m_WorldId);
    }
  }

  void PhysicsWorld::Update(f64 deltaTime) {
    PROFILE_SCOPE();
    b2World_Step(m_WorldId, static_cast<float>(deltaTime), PHYSICS_STEPS);
  }

  b2BodyId PhysicsWorld::CreateBody(const b2BodyDef &bodyDef) {
    b2BodyId bodyId = b2CreateBody(m_WorldId, &bodyDef);
    return bodyId;
  }
}
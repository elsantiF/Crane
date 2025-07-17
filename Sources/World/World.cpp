#include "World.hpp"
#include "Core/Profiler.hpp"
#include "Entity.hpp"

namespace Crane::World {
  void World::FixedUpdate(f64 deltaTime) {
    PROFILE_SCOPE();
    m_SystemManager.FixedUpdateSystems(deltaTime);
  }

  void World::Update(f64 deltaTime) {
    PROFILE_SCOPE();
    m_SystemManager.UpdateSystems(deltaTime);
  }

  Entity World::CreateEntity() {
    PROFILE_SCOPE();
    auto entity = m_Registry.create();
    return Entity(*this, entity);
  }
}
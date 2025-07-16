#include "World.hpp"
#include "Core/Profiler.hpp"
#include "Entity.hpp"
#include "Systems/RenderingSystem.hpp"

namespace Crane::World {
  void World::FixedUpdate(f64 deltaTime) {
    PROFILE_SCOPE();
    for (auto &system : m_FixedUpdateSystems) {
      system->FixedUpdate(*this, deltaTime);
    }
  }

  void World::Update(f64 deltaTime) {
    PROFILE_SCOPE();
    for (auto &system : m_UpdateSystems) {
      system->Update(*this, deltaTime);
    }
  }

  Entity World::CreateEntity() {
    PROFILE_SCOPE();
    auto entity = m_Registry.create();
    return Entity(*this, entity);
  }
}
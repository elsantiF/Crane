#include "World.hpp"
#include "Base/Profiler.hpp"
#include "Scene/Entity/Entity.hpp"

namespace Crane::Scene {
  void World::FixedUpdate(f64 deltaTime) {
    PROFILE_SCOPE();
    m_SystemManager.FixedUpdateSystems(deltaTime);
  }

  void World::Update(f64 deltaTime) {
    PROFILE_SCOPE();
    m_SystemManager.UpdateSystems(deltaTime);
  }

  Scene::Entity World::CreateEntity() {
    PROFILE_SCOPE();
    auto entity = m_Registry.create();
    return Entity(this, entity);
  }
}
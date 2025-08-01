#include "World.hpp"
#include "Base/Profiler.hpp"

namespace Crane::Scene {
  void World::FixedUpdate(f64 deltaTime) {
    PROFILE_SCOPE();
    for (auto &[type, system] : m_FixedUpdateSystems) {
      system->FixedUpdate(deltaTime);
    }
  }

  void World::Update(f64 deltaTime) {
    PROFILE_SCOPE();
    for (auto &[type, system] : m_UpdateSystems) {
      system->Update(deltaTime);
    }
  }

  Scene::Entity World::CreateEntity() {
    return m_Registry.create();
  }

  void World::DestroyEntity(Entity entity) {
    m_Registry.destroy(entity);
  }

  entt::registry &World::GetRegistry() {
    return m_Registry;
  }

  entt::dispatcher &World::GetDispatcher() {
    return m_Dispatcher;
  }
}
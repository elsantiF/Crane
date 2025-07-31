#include "World.hpp"
#include "Base/Profiler.hpp"

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
    return m_Registry.create();
  }

  void World::DestroyEntity(Entity entity) {
    m_Registry.destroy(entity);
  }

  entt::registry &World::GetRegistry() {
    return m_Registry;
  }

  Core::Systems::SystemManager &World::GetSystemManager() {
    return m_SystemManager;
  }
}
#include "World.hpp"
#include "Base/Config.hpp"
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
    PROFILE_SCOPE();
    return m_Registry.create();
  }

  void World::DestroyEntity(Entity entity) {
    PROFILE_SCOPE();
    m_Registry.destroy(entity);
  }

  entt::registry &World::GetRegistry() {
    return m_Registry;
  }

  f32 World::GetPixelsPerMeter() const {
    return PIXELS_PER_METER;
  }

  Core::Systems::SystemManager &World::GetSystemManager() {
    return m_SystemManager;
  }
}
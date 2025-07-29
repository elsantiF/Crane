#include "SystemManager.hpp"
#include "Base/Profiler.hpp"

namespace Crane::Core::Systems {
  void SystemManager::UpdateSystems(f64 deltaTime) {
    PROFILE_SCOPE();
    for (auto &system : m_UpdateSystems) {
      system->Update(m_World, deltaTime);
    }
  }

  void SystemManager::FixedUpdateSystems(f64 deltaTime) {
    PROFILE_SCOPE();
    for (auto &system : m_FixedUpdateSystems) {
      system->FixedUpdate(m_World, deltaTime);
    }
  }
}
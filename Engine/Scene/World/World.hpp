#pragma once

#include "Base/Config.hpp"
#include "Base/Types.hpp"
#include "Core/Systems/SystemManager.hpp"
#include <entt/entt.hpp>

namespace Crane::Scene {
  class Entity;

  class World {
  public:
    World() : m_SystemManager(*this) {};
    ~World() = default;

    World(const World &) = delete;
    World &operator=(const World &) = delete;

    void FixedUpdate(f64 deltaTime);
    void Update(f64 deltaTime);

    Scene::Entity CreateEntity();

    entt::registry &GetRegistry() {
      return m_Registry;
    }

    f32 GetPixelsPerMeter() const {
      return PIXELS_PER_METER;
    }

    Core::Systems::SystemManager &GetSystemManager() {
      return m_SystemManager;
    }

  private:
    entt::registry m_Registry;
    Core::Systems::SystemManager m_SystemManager;
  };
}
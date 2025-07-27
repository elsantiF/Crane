#pragma once

#include "Core/Config.hpp"
#include "Core/Types.hpp"
#include "Systems/SystemManager.hpp"
#include <entt/entt.hpp>

namespace Crane::World {
  class Entity;

  class World {
  public:
    World() : m_SystemManager(*this) {};
    ~World() = default;

    World(const World &) = delete;
    World &operator=(const World &) = delete;

    void FixedUpdate(f64 deltaTime);
    void Update(f64 deltaTime);

    Entity CreateEntity();

    entt::registry &GetRegistry() {
      return m_Registry;
    }

    f32 GetPixelsPerMeter() const {
      return PIXELS_PER_METER;
    }

    Systems::SystemManager &GetSystemManager() {
      return m_SystemManager;
    }

  private:
    entt::registry m_Registry;
    Systems::SystemManager m_SystemManager;
  };
}
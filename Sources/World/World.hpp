#pragma once

#include "Core/Config.hpp"
#include "Core/Types.hpp"
#include "Physics/World.hpp"
#include "Systems/SystemManager.hpp"
#include <entt/entt.hpp>

namespace Crane::World {
  class Entity;

  class World {
  public:
    World() : m_PhysicsWorld(), m_SystemManager(*this) {};
    ~World() = default;

    World(const World &) = delete;
    World &operator=(const World &) = delete;

    void FixedUpdate(f64 deltaTime);
    void Update(f64 deltaTime);

    Entity CreateEntity();

    Physics::World &GetPhysicsWorld() {
      return m_PhysicsWorld;
    }

    const Physics::World &GetPhysicsWorld() const {
      return m_PhysicsWorld;
    }

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
    Physics::World m_PhysicsWorld;
    entt::registry m_Registry;
    Systems::SystemManager m_SystemManager;
  };
}
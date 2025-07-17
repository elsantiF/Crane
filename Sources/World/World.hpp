#pragma once

#include "Core/Types.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Systems/SystemManager.hpp"
#include <box2d/box2d.h>
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

    Physics::PhysicsWorld &GetPhysicsWorld() { return m_PhysicsWorld; }
    const Physics::PhysicsWorld &GetPhysicsWorld() const { return m_PhysicsWorld; }
    entt::registry &GetRegistry() { return m_Registry; }
    f32 GetPixelsPerMeter() const { return PIXELS_PER_METER; }
    Systems::SystemManager &GetSystemManager() { return m_SystemManager; }

  private:
    void WakeUpBodies();

  private:
    Physics::PhysicsWorld m_PhysicsWorld;
    entt::registry m_Registry;
    Systems::SystemManager m_SystemManager;

    const f32 PIXELS_PER_METER = 30.0f;
  };
}
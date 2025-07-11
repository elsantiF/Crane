#pragma once

#include "Core/Types.hpp"
#include "Physics/PhysicsWorld.hpp"
#include <box2d/box2d.h>
#include <entt/entt.hpp>

namespace Crane::World {
  class Entity;

  class World {
  public:
    World();
    ~World() = default;

    World(const World &) = delete;
    World &operator=(const World &) = delete;

    void Update(f64 deltaTime);

    Entity CreateEntity();

    Physics::PhysicsWorld *GetPhysicsWorld() const { return m_PhysicsWorld.get(); }
    entt::registry &GetRegistry() { return m_Registry; }

  private:
    Scope<Physics::PhysicsWorld> m_PhysicsWorld;
    entt::registry m_Registry;

    const f32 PIXELS_PER_METER = 30.0f;
  };
}
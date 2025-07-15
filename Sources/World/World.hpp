#pragma once

#include "Core/Types.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Systems/ISystem.hpp"
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
    void Render(Graphics::IRenderer &renderer);

    Entity CreateEntity();

    Physics::PhysicsWorld &GetPhysicsWorld() { return m_PhysicsWorld; }
    const Physics::PhysicsWorld &GetPhysicsWorld() const { return m_PhysicsWorld; }
    entt::registry &GetRegistry() { return m_Registry; }
    f32 GetPixelsPerMeter() const { return PIXELS_PER_METER; }

  private:
    void WakeUpBodies();

  private:
    Physics::PhysicsWorld m_PhysicsWorld;
    entt::registry m_Registry;
    Vector<Scope<Systems::IUpdateSystem>> m_UpdateSystems;
    Vector<Scope<Systems::IRenderSystem>> m_RenderSystems;

    const f32 PIXELS_PER_METER = 30.0f;
  };
}
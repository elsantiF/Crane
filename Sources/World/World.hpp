#pragma once

#include "Core/Types.hpp"
#include <box2d/box2d.h>
#include <entt/entt.hpp>

class World {
public:
  World();
  ~World();

  void Update(f64 deltaTime);

  b2WorldId GetWorldId() const { return m_WorldId; }
  entt::registry &GetRegistry() { return m_Registry; }

private:
  b2WorldId m_WorldId;
  entt::registry m_Registry;

  const f32 PIXELS_PER_METER = 30.0f;
  const i32 PHYSICS_STEPS = 4;
};
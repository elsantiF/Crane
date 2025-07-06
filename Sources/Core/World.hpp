#pragma once

#include "Core/Types.hpp"
#include <box2d/box2d.h>
#include <entt/entt.hpp>

class World {
public:
  World();
  ~World();

  void Update(f32 deltaTime);

  b2WorldId GetWorldId() const { return worldId; }
  entt::registry &GetRegistry() { return registry; }

private:
  b2WorldId worldId;
  entt::registry registry;

  const f32 PIXELS_PER_METER = 30.0f;
  const i32 PHYSICS_STEPS = 4;
};
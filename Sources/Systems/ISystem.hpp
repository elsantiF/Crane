#pragma once

#include "Core/Types.hpp"
#include "World/World.hpp"

namespace Crane::Systems {
  class ISystem {
  public:
    virtual ~ISystem() = default;

    virtual void Update(World::World &world, f64 deltaTime) = 0;
  };
}
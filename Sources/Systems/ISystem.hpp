#pragma once

#include "Core/Types.hpp"

namespace Crane::World {
  class World;
}

namespace Crane::Graphics {
  class IRenderer;
}

namespace Crane::Systems {
  class ISystem {
  public:
    virtual ~ISystem() = default;

    virtual void Initialize(World::World &world) = 0;
    virtual void Shutdown(World::World &world) = 0;
  };

  class IFixedUpdateSystem : public ISystem {
  public:
    virtual ~IFixedUpdateSystem() = default;

    virtual void FixedUpdate(World::World &world, f64 deltaTime) = 0;
  };

  class IUpdateSystem : public ISystem {
  public:
    virtual ~IUpdateSystem() = default;

    virtual void Update(World::World &world, f64 deltaTime) = 0;
  };
}
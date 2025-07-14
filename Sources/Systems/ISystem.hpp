#pragma once

#include "Core/Types.hpp"

namespace Crane::World {
  class World;
}

namespace Crane::Graphics {
  class IRenderer;
}

namespace Crane::Systems {
  class IUpdateSystem {
  public:
    virtual ~IUpdateSystem() = default;

    virtual void Update(World::World &world, f64 deltaTime) = 0;
  };

  class IRenderSystem {
  public:
    virtual ~IRenderSystem() = default;

    virtual void Render(World::World &world, Graphics::IRenderer &renderer) = 0;
  };
}
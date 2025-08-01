#pragma once

#include "Base/Types.hpp"

namespace Crane::Scene {
  class World;
}

namespace Crane::Graphics {
  class IRenderer;
}

namespace Crane::Systems {
  class ISystem {
  public:
    virtual ~ISystem() = default;
  };

  class IFixedUpdateSystem : public ISystem {
  public:
    virtual ~IFixedUpdateSystem() = default;

    virtual void FixedUpdate(Scene::World &world, f64 deltaTime) = 0;
  };

  class IUpdateSystem : public ISystem {
  public:
    virtual ~IUpdateSystem() = default;

    virtual void Update(Scene::World &world, f64 deltaTime) = 0;
  };
}
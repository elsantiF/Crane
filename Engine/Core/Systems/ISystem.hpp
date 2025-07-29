#pragma once

#include "Base/Types.hpp"

namespace Crane::Scene {
  class World;
}

namespace Crane::Graphics {
  class IRenderer;
}

namespace Crane::Core::Systems {
  class ISystem {
  public:
    virtual ~ISystem() = default;

    virtual void Initialize(Scene::World &world) = 0;
    virtual void Shutdown(Scene::World &world) = 0;
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

  class IRenderSystem : public ISystem {
  public:
    virtual ~IRenderSystem() = default;

    virtual void Render(Scene::World &world, Graphics::IRenderer &renderer) = 0;
  };
}
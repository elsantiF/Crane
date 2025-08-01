#pragma once

#include "Base/Types.hpp"

namespace Crane::Scene {
  class World;
}

namespace Crane::Systems {
  class ISystem {
  public:
    explicit ISystem(Scene::World &world) : m_World(world) {}
    virtual ~ISystem() = default;

  protected:
    Scene::World &m_World;
  };

  class IFixedUpdateSystem : public ISystem {
  public:
    explicit IFixedUpdateSystem(Scene::World &world) : ISystem(world) {}
    virtual ~IFixedUpdateSystem() = default;

    virtual void FixedUpdate(f64 deltaTime) = 0;
  };

  class IUpdateSystem : public ISystem {
  public:
    explicit IUpdateSystem(Scene::World &world) : ISystem(world) {}
    virtual ~IUpdateSystem() = default;

    virtual void Update(f64 deltaTime) = 0;
  };
}
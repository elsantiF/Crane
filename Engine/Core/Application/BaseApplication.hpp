#pragma once

#include "ApplicationInfo.hpp"
#include "Base/Types.hpp"
#include "Scene/World.hpp"
#include <entt/signal/dispatcher.hpp>

namespace Crane {
  class BaseApplication {
  public:
    explicit BaseApplication(const ApplicationInfo &info) : m_AppInfo(info), m_Running(false) {}
    virtual ~BaseApplication() = default;

    void Initialize();
    void Run();

    Scene::World &GetWorld() {
      return *m_World;
    }

  protected:
    virtual void Cleanup() = 0;
    virtual void PlatformInitialize() = 0;
    virtual void HandleEvents() = 0;

    virtual void OnInitialize() = 0;

    void FixedUpdate(f64 deltaTime);
    void Update(f64 deltaTime);
    virtual void Render() = 0;

  protected:
    const ApplicationInfo &m_AppInfo;
    Scope<Scene::World> m_World;
    bool m_Running;

    const f64 PHYSICS_TIMESTEP = 1.0 / 60.0;
  };
}
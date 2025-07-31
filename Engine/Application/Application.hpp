#pragma once

#include "ApplicationInfo.hpp"
#include "Base/Types.hpp"
#include "Graphics/IRenderer.hpp"
#include "Graphics/SDLWindow.hpp"
#include "Scene/World.hpp"
#include <entt/signal/dispatcher.hpp>

namespace Crane {
  class Application {
  public:
    explicit Application(const ApplicationInfo &info)
        : m_AppInfo(info), m_Window(nullptr), m_Renderer(nullptr), m_RenderingSystem(nullptr), m_Running(false) {}
    ~Application() {
      Cleanup();
    }

    bool Initialize();
    void Run();

    Scene::World &GetWorld() {
      return *m_World;
    }

    entt::dispatcher &GetDispatcher() {
      return m_Dispatcher;
    }

  private:
    bool InitializeSDL();
    void HandleEvents();
    void FixedUpdate(f64 deltaTime);
    void Update(f64 deltaTime);
    void Render();
    void Cleanup();

  protected:
    virtual void OnInitialize() = 0;
    virtual void OnPreFixedUpdate() = 0;
    virtual void OnPostFixedUpdate() = 0;
    virtual void OnPreUpdate() = 0;
    virtual void OnPostUpdate() = 0;
    virtual void OnPreRender() = 0;
    virtual void OnPostRender() = 0;
    virtual void OnImGui() = 0;

  protected: // TODO: Protected is not ideal, but necessary for now
    const ApplicationInfo &m_AppInfo;
    Scope<Graphics::SDLWindow> m_Window;
    Scope<Graphics::IRenderer> m_Renderer;
    Scope<Scene::World> m_World;
    Scope<Systems::IRenderSystem> m_RenderingSystem;
    entt::dispatcher m_Dispatcher;
    bool m_Running;

    f64 m_DeltaTime = 0.0;
    f64 m_Accumulator = 0.0;

    const f64 PHYSICS_TIMESTEP = 1.0 / 60.0;
  };
}
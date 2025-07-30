#pragma once

#include "ApplicationInfo.hpp"
#include "Base/Types.hpp"
#include "Graphics/IRenderer.hpp"
#include "Scene/World/World.hpp"
#include <entt/entt.hpp>
#include <SDL3/SDL.h>

namespace Crane::Core {
  class Application {
  public:
    explicit Application(const ApplicationInfo &info) : m_Window(nullptr), m_Renderer(nullptr), m_Running(false), m_AppInfo(info) {}
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
    void FixedUpdate();
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
    SDL_Window *m_Window;
    Scope<Graphics::IRenderer> m_Renderer;
    Scope<Scene::World> m_World;
    Scope<Systems::IRenderSystem> m_RenderingSystem;
    entt::dispatcher m_Dispatcher;
    bool m_Running;

    f64 m_DeltaTime = 0.0;
    f64 m_Accumulator = 0.0;
  };
}
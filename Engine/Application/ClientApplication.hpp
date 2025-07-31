#pragma once

#include "Core/Application/BaseApplication.hpp"
#include "Graphics/IRenderer.hpp"
#include "Graphics/SDLWindow.hpp"
#include "Systems/ISystem.hpp"

namespace Crane {
  class ClientApplication : public BaseApplication {
  public:
    ClientApplication(const ApplicationInfo &appInfo) : BaseApplication(appInfo) {}

  protected:
    void PlatformInitialize() override;
    void HandleEvents() override;
    void Cleanup() override;

    virtual void OnImGui() = 0;

    void Render() override;

  protected: // TODO: This should be private, but needs to be accessible
    Scope<Graphics::SDLWindow> m_Window;
    Scope<Graphics::IRenderer> m_Renderer;
    Scope<Systems::IRenderSystem> m_RenderingSystem;
    entt::dispatcher m_Dispatcher;
  };
}
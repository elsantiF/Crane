#pragma once

#include "Core/Application/BaseApplication.hpp"
#include "Graphics/IRenderer.hpp"
#include "Graphics/RenderPipeline.hpp"
#include "Graphics/SDLWindow.hpp"

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
    Scope<Graphics::RenderPipeline> m_RenderPipeline;
    entt::dispatcher m_Dispatcher;
  };
}
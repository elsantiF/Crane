#pragma once

#include "Core/Application/BaseApplication.hpp"
#include "Graphics/Renderer/IRenderer.hpp"
#include "Graphics/Window/SDLWindow.hpp"

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

    Id m_RenderPipelineId = 0;
    Id m_RenderPassId = 0;
  };
}
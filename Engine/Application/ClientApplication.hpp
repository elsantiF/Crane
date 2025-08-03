#pragma once

#include "Core/Application/BaseApplication.hpp"
#include "Graphics/Pipeline/RenderPipeline.hpp"
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
    Scope<Graphics::RenderPipeline> m_RenderPipeline;
  };
}
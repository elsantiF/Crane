#include "ClientApplication.hpp"
#include "Base/Assert.hpp"
#include "Base/Logger.hpp"
#include "Base/Profiler.hpp"
#include "Events/Events.hpp"
#include "Graphics/Renderer/SDLGPU/SDLGPURenderer.hpp"
#include <imgui_impl_sdl3.h>
#include <SDL3/SDL.h>

namespace Crane {
  void ClientApplication::PlatformInitialize() {
    PROFILE_SCOPE();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
      Assert::Crash("Failed to initialize SDL");
    }

    m_Window = MakeScope<Graphics::SDLWindow>(m_AppInfo.appName, m_AppInfo.window.width, m_AppInfo.window.height);
    auto renderer = MakeScope<Graphics::SDLGPURenderer::SDLGPURenderer>(m_Window->GetHandle());

    m_RenderPipeline = MakeScope<Graphics::RenderPipeline>(std::move(renderer));
  }

  void ClientApplication::HandleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL3_ProcessEvent(&event);

      switch (event.type) {
      case SDL_EVENT_QUIT: m_Running = false; break;
      case SDL_EVENT_KEY_DOWN:
        if (event.key.key == SDLK_ESCAPE) {
          m_Running = false;
        }

        if (!event.key.repeat) {
          m_World->GetDispatcher().trigger(Events::KeyDown{event.key.key});
        }
        break;
      case SDL_EVENT_KEY_UP:
        if (!event.key.repeat) {
          m_World->GetDispatcher().trigger(Events::KeyUp{event.key.key});
        }
        break;
      case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        auto position = Math::Vec2f(event.button.x, event.button.y);
        switch (event.button.button) {
        case SDL_BUTTON_LEFT:   m_World->GetDispatcher().trigger(Events::MouseClick{position, Events::MouseButton::Left}); break;
        case SDL_BUTTON_RIGHT:  m_World->GetDispatcher().trigger(Events::MouseClick{position, Events::MouseButton::Right}); break;
        case SDL_BUTTON_MIDDLE: m_World->GetDispatcher().trigger(Events::MouseClick{position, Events::MouseButton::Middle}); break;
        default:
          // TODO: Handle other mouse buttons if needed
          break;
        }
        break;
      }
      }
    }

    m_World->GetDispatcher().update();
  }

  void ClientApplication::Render() {
    PROFILE_SCOPE();
    m_RenderPipeline->Render(*m_World);
    m_RenderPipeline->RenderImGui([this]() {
      OnImGui();
    });
    m_RenderPipeline->Present();
  }

  void ClientApplication::Cleanup() {
    m_RenderPipeline.reset();
    m_Window.reset();
    SDL_Quit();
    Logger::Info("Application cleaned up");
    Logger::Shutdown();
  }
}
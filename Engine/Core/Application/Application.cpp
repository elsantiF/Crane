#include "Application.hpp"
#include "Base/Logger.hpp"
#include "Base/Profiler.hpp"
#include "Events/Events.hpp"
#include "Graphics/RenderingSystem.hpp"
#include "Graphics/SDLRenderer/SDLRenderer.hpp"
#include <imgui_impl_sdl3.h>
#include <SDL3/SDL.h>

namespace Crane::Core {
  bool Application::Initialize() {
    PROFILE_SCOPE();
    Logger::Initialize();
    if (!InitializeSDL()) {
      return false;
    }
    m_World = MakeScope<Scene::World>();
    m_RenderingSystem = MakeScope<Graphics::RenderingSystem>();
    m_Running = true;
    OnInitialize();
    return true;
  }

  bool Application::InitializeSDL() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
      Logger::Error("SDL Initialization Error: {}", SDL_GetError());
      return false;
    }

    m_Window = MakeScope<Graphics::SDLWindow>(m_AppInfo.appName, m_AppInfo.window.width, m_AppInfo.window.height);

    m_Renderer = MakeScope<Graphics::SDLRenderer::SDLRenderer>(m_Window->GetHandle());
    if (!m_Renderer->Initialize()) {
      Logger::Error("SDLRenderer Initialization Error: {}", SDL_GetError());
      return false;
    }

    Logger::Info("SDL Initialized successfully");

    return true;
  }

  void Application::HandleEvents() {
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
          m_Dispatcher.trigger(Events::KeyDown{event.key.key});
        }
        break;
      case SDL_EVENT_KEY_UP:
        if (!event.key.repeat) {
          m_Dispatcher.trigger(Events::KeyUp{event.key.key});
        }
        break;
      case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        auto position = Math::Vec2f(event.button.x, event.button.y);
        switch (event.button.button) {
        case SDL_BUTTON_LEFT:   m_Dispatcher.trigger(Events::MouseClick{position, Events::MouseButton::Left}); break;
        case SDL_BUTTON_RIGHT:  m_Dispatcher.trigger(Events::MouseClick{position, Events::MouseButton::Right}); break;
        case SDL_BUTTON_MIDDLE: m_Dispatcher.trigger(Events::MouseClick{position, Events::MouseButton::Middle}); break;
        default:
          // TODO: Handle other mouse buttons if needed
          break;
        }
        break;
      }
      }
    }

    m_Dispatcher.update();
  }

  void Application::FixedUpdate(f64 deltaTime) {
    PROFILE_SCOPE();
    OnPreFixedUpdate();
    m_World->FixedUpdate(deltaTime);
    OnPostFixedUpdate();
  }

  void Application::Update(f64 deltaTime) {
    PROFILE_SCOPE();
    OnPreUpdate();
    m_World->Update(deltaTime);
    OnPostUpdate();
  }

  void Application::Render() {
    PROFILE_SCOPE();

    m_Renderer->BeginFrame();
    m_Renderer->Clear(Graphics::Colors::CLEAR_COLOR);
    OnPreRender();
    m_RenderingSystem->Render(*m_World, *m_Renderer);
    OnPostRender();

    m_Renderer->BeginImGuiFrame();
    OnImGui();
    m_Renderer->EndImGuiFrame();

    m_Renderer->Present();
    m_Renderer->EndFrame();
  }

  void Application::Run() {
    f64 lastTime = SDL_GetTicks() / 1000.0;
    while (m_Running) {
      const f64 currentTime = SDL_GetTicks() / 1000.0;
      m_DeltaTime = currentTime - lastTime;
      m_Accumulator += m_DeltaTime;

      HandleEvents();

      while (m_Accumulator >= PHYSICS_TIMESTEP) {
        FixedUpdate(PHYSICS_TIMESTEP);
        m_Accumulator -= PHYSICS_TIMESTEP;
      }

      Update(m_DeltaTime);
      Render();

      lastTime = currentTime;
    }
  }

  void Application::Cleanup() {
    m_Window.reset();
    SDL_Quit();
    Logger::Info("Application cleaned up");
    Logger::Shutdown();
  }
}
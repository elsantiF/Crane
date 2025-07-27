#include "Application.hpp"
#include "Core/Config.hpp"
#include "Core/Logger.hpp"
#include "Core/Profiler.hpp"
#include "Editor/EntityDisplay.hpp"
#include "Events/KeyDown.hpp"
#include "Events/KeyUp.hpp"
#include "Events/MouseClick.hpp"
#include "Graphics/SDLRenderer/SDLRenderer.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/RenderingSystem.hpp"
#include "World/Entity.hpp"
#include <imgui.h>
#include <imgui_impl_sdl3.h>

namespace Crane::Application {
  bool Application::Initialize() {
    PROFILE_SCOPE();
    Logger::Initialize();
    if (!InitializeSDL()) {
      return false;
    }
    m_World = MakeScope<World::World>();
    m_World->GetSystemManager().AddSystem<Systems::PhysicsSystem>(Math::Vec2f{0.0f, -9.81f});
    m_RenderingSystem = MakeScope<Systems::RenderingSystem>();
    InitializeImGui();
    m_Running = true;
    OnInitialize();
    return true;
  }

  bool Application::InitializeSDL() {
    PROFILE_SCOPE();
    if (!SDL_Init(SDL_INIT_VIDEO)) {
      Logger::Error(std::format("SDL Initialization Error: {}", SDL_GetError()));
      return false;
    }

    m_Window = SDL_CreateWindow("Crane", Core::WINDOW_WIDTH, Core::WINDOW_HEIGHT, NULL);
    if (!m_Window) {
      Logger::Error(std::format("SDL_CreateWindow Error: {}", SDL_GetError()));
      return false;
    }

    m_Renderer = MakeScope<Graphics::SDLRenderer::SDLRenderer>(m_Window);
    if (!m_Renderer->Initialize()) {
      Logger::Error(std::format("SDLRenderer Initialization Error: {}", SDL_GetError()));
      return false;
    }

    Logger::Info("SDL Initialized successfully");

    return true;
  }

  void Application::InitializeImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    m_Renderer->InitializeImGui();
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
          m_Dispatcher.trigger(Events::KeyDownEvent{event.key.key});
        }
        break;
      case SDL_EVENT_KEY_UP:
        if (!event.key.repeat) {
          m_Dispatcher.trigger(Events::KeyUpEvent{event.key.key});
        }
        break;
      case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        auto position = Math::Vec2f(event.button.x, event.button.y);
        switch (event.button.button) {
        case SDL_BUTTON_LEFT:   m_Dispatcher.trigger(Events::MouseClickEvent{position, Events::MouseButton::Left}); break;
        case SDL_BUTTON_RIGHT:  m_Dispatcher.trigger(Events::MouseClickEvent{position, Events::MouseButton::Right}); break;
        case SDL_BUTTON_MIDDLE: m_Dispatcher.trigger(Events::MouseClickEvent{position, Events::MouseButton::Middle}); break;
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

  void Application::FixedUpdate() {
    PROFILE_SCOPE();
    OnPreFixedUpdate();
    m_World->FixedUpdate(Core::PHYSICS_TIMESTEP);
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
    m_Renderer->Clear(Colors::CLEAR_COLOR);
    OnPreRender();
    m_RenderingSystem->Render(*m_World, *m_Renderer);
    OnPostRender();
    m_Renderer->BeginImGuiFrame();

    ImGui::Begin("Stats");
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    ImGui::Text("Delta: %.3f ms", m_DeltaTime * 1000.0);
    ImGui::Text("ESC to exit");

    auto &registry = m_World->GetRegistry();
    Crane::Editor::EntityDisplay::DrawEntityList(registry);
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

      while (m_Accumulator >= Core::PHYSICS_TIMESTEP) {
        FixedUpdate();
        m_Accumulator -= Core::PHYSICS_TIMESTEP;
      }

      Update(m_DeltaTime);
      Render();

      lastTime = currentTime;
    }
  }

  void Application::Cleanup() {
    m_Renderer->ShutdownImGui();
    ImGui::DestroyContext();

    if (m_Window) {
      SDL_DestroyWindow(m_Window);
    }

    SDL_Quit();
    Logger::Info("Application cleaned up");
    Logger::Shutdown();
  }
}
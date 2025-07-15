#include "Application.hpp"
#include "Config.hpp"
#include "Editor/EntityDisplay.hpp"
#include "Graphics/SDLRenderer/SDLRenderer.hpp"
#include "Profiler.hpp"
#include "World/Entity.hpp"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <iostream>

namespace Crane::Core {
  bool Application::Initialize() {
    PROFILE_SCOPE();
    if (!InitializeSDL()) {
      return false;
    }
    m_World = MakeScope<World::World>();
    m_RenderingSystem = MakeScope<Systems::RenderingSystem>(*m_Renderer, *m_World);
    InitializeImGui();
    m_Running = true;
    OnInitialize();
    return true;
  }

  bool Application::InitializeSDL() {
    PROFILE_SCOPE();
    if (!SDL_Init(SDL_INIT_VIDEO)) {
      std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
      return false;
    }

    m_Window = SDL_CreateWindow("Crane", WINDOW_WIDTH, WINDOW_HEIGHT, NULL);
    if (!m_Window) {
      std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
      return false;
    }

    m_Renderer = MakeScope<Graphics::SDLRenderer::SDLRenderer>(m_Window);
    if (!m_Renderer->Initialize()) {
      std::cerr << "SDLRenderer Initialization Error: " << SDL_GetError() << std::endl;
      return false;
    }

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
        break;
      }
    }
  }

  void Application::FixedUpdate() {
    PROFILE_SCOPE();
    OnPreFixedUpdate();
    m_World->FixedUpdate(PHYSICS_TIMESTEP);
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
    m_RenderingSystem->Render();
    OnPostRender();
    m_Renderer->BeginImGuiFrame();
    ImGui::NewFrame();

    ImGui::Begin("Stats");
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    ImGui::Text("Delta: %.3f ms", m_DeltaTime * 1000.0);
    ImGui::Text("ESC to exit");
    ImGui::End();

    auto &registry = m_World->GetRegistry();
    Crane::Editor::EntityDisplay::DrawEntityList(registry);
    OnImGui();

    ImGui::Render();
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
        FixedUpdate();
        m_Accumulator -= PHYSICS_TIMESTEP;
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
  }
}
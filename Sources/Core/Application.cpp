#include "Application.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Editor/EntityDisplay.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/SDLRenderer/SDLRenderer.hpp"
#include "Physics/PhysicsFactory.hpp"
#include "World/Entity.hpp"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <iostream>

namespace Crane::Core {
  bool Application::Initialize() {
    if (!InitializeSDL()) {
      return false;
    }
    InitializeImGui();
    InitializeEntities();
    m_Running = true;
    return true;
  }

  bool Application::InitializeSDL() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
      std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
      return false;
    }

    m_Window = SDL_CreateWindow("Crane", 1600, 900, NULL);
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

  void Application::InitializeEntities() {
    auto &physicsWorld = m_World.GetPhysicsWorld();

    // Create ground body
    World::Entity ground = m_World.CreateEntity();
    {
      ground.AddComponent<Components::Transform>(Math::Vec2f{512.0f, 725.0f});
      ground.AddComponent<Components::Renderable>(Graphics::Color{0, 255, 0, 255}, 1000.0f, 50.0f);

      b2BodyId bodyId = Physics::PhysicsFactory::CreateBoxBody(physicsWorld, {512, 700, 1000, 50, Physics::BodyType::Static}, PIXELS_PER_METER);
      ground.AddComponent<Components::Rigidbody>(bodyId);
    }

    // Create a dynamic box body
    World::Entity box = m_World.CreateEntity();
    {
      box.AddComponent<Components::Transform>(Math::Vec2f{400.0f, 100.0f});
      box.AddComponent<Components::Renderable>(Graphics::Color{255, 0, 0, 255}, 40.0f, 40.0f);

      b2BodyId bodyId = Physics::PhysicsFactory::CreateBoxBody(physicsWorld, {400, 100, 40, 40, Physics::BodyType::Dynamic}, PIXELS_PER_METER);
      box.AddComponent<Components::Rigidbody>(bodyId);
    }

    World::Entity blueBox = m_World.CreateEntity();
    {
      blueBox.AddComponent<Components::Transform>(Math::Vec2f{600.0f, 100.0f});
      blueBox.AddComponent<Components::Renderable>(Graphics::Color{0, 0, 255, 255}, 40.0f, 40.0f);
    }
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

  void Application::Update(f64 deltaTime) { m_World.Update(deltaTime); }

  void Application::Render() {
    m_Renderer->BeginFrame();
    m_Renderer->Clear(Graphics::Color{0.05f, 0.05f, 0.05f, 1.0f});

    m_World.Render(*m_Renderer);

    m_Renderer->BeginImGuiFrame();
    ImGui::NewFrame();

    ImGui::Begin("Stats");
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    ImGui::Text("Delta: %.3f ms", m_DeltaTime * 1000.0);
    ImGui::Text("ESC to exit");
    ImGui::End();

    auto &registry = m_World.GetRegistry();
    Crane::Editor::EntityDisplay::DrawEntityList(registry);

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

      if (m_DeltaTime < DELTA_LOW_CAP) {
        m_DeltaTime = DELTA_LOW_CAP;
      } else if (m_DeltaTime > DELTA_HIGH_CAP) {
        m_DeltaTime = DELTA_HIGH_CAP;
      }

      HandleEvents();
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
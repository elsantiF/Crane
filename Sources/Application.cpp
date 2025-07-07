#include "Application.hpp"
#include "Components/Renderable.hpp"
#include "Components/Rigidbody.hpp"
#include "Components/Transform.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/Rect.hpp"
#include "Graphics/SDLRenderer/SDLRenderer.hpp"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <iostream>

bool Application::Initialize() {
  if (!InitializeSDL()) {
    return false;
  }
  InitializeImGui();
  InitializeEntities();
  m_Running = true;
  m_LastTime = SDL_GetTicks() / 1000.0;
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

  m_Renderer = MakeScope<SDLRenderer>(m_Window);
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
  auto &registry = m_World.GetRegistry();
  auto worldId = m_World.GetWorldId();
  // Create ground body
  {
    auto ground = registry.create();
    registry.emplace<Transform>(ground, 512, 700);
    registry.emplace<Renderable>(ground, Color{0, 255, 0, 255}, 1000, 50);
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    bodyDef.position = {512 / PIXELS_PER_METER, 700 / PIXELS_PER_METER};
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

    b2Polygon box = b2MakeBox(500 / PIXELS_PER_METER, 25 / PIXELS_PER_METER);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(bodyId, &shapeDef, &box);

    registry.emplace<Rigidbody>(ground, bodyId);
  }

  // Create a dynamic box body
  {
    auto box = registry.create();
    registry.emplace<Transform>(box, 400.0f, 100.0f);
    registry.emplace<Renderable>(box, Color{255, 0, 0, 255}, 40.0f, 40.0f);
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = {400.f / PIXELS_PER_METER, 100.f / PIXELS_PER_METER};
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

    b2Polygon boxShape = b2MakeBox(20 / PIXELS_PER_METER, 20 / PIXELS_PER_METER);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);

    registry.emplace<Rigidbody>(box, bodyId);
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
  m_Renderer->Clear(Color{30, 30, 30, 255});

  auto &registry = m_World.GetRegistry();
  auto view = registry.view<Transform, Renderable>();
  for (auto entity : view) {
    const auto &transform = view.get<Transform>(entity);
    const auto &renderable = view.get<Renderable>(entity);

    Rect rect{transform.x - renderable.width / 2.0f, transform.y - renderable.height / 2.0f, renderable.width, renderable.height};

    m_Renderer->DrawRect(rect, renderable.color);
  }

  m_Renderer->BeginImGuiFrame();
  ImGui::NewFrame();

  ImGui::Begin("Stats");
  ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
  ImGui::Text("Delta: %.3f ms", m_DeltaTime * 1000.0);
  ImGui::Text("ESC to exit");
  ImGui::End();

  ImGui::Render();
  m_Renderer->EndImGuiFrame();
  m_Renderer->Present();
  m_Renderer->EndFrame();
}

void Application::Run() {
  while (m_Running) {
    HandleEvents();
    Update(m_DeltaTime);
    Render();

    const f64 currentTime = SDL_GetTicks() / 1000.0;
    m_DeltaTime = currentTime - m_LastTime;
    m_LastTime = currentTime;
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
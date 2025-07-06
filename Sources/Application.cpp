#include "Application.hpp"
#include "Components/Renderable.hpp"
#include "Components/Rigidbody.hpp"
#include "Components/Transform.hpp"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <iostream>

bool Application::Initialize() {
  if (!InitializeSDL()) {
    return false;
  }
  InitializeImGui();
  InitializeBox2D();
  InitializeEntities();
  running = true;
  return true;
}

bool Application::InitializeSDL() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return false;
  }

  window = SDL_CreateWindow("Crane", 1600, 900, NULL);
  if (!window) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    return false;
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    return false;
  }

  return true;
}

void Application::InitializeImGui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGui::StyleColorsDark();
  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);
}

void Application::InitializeBox2D() {
  b2WorldDef worldDef = b2DefaultWorldDef();
  worldDef.gravity = {0.0f, 9.81f};
  worldId = b2CreateWorld(&worldDef);
}

void Application::InitializeEntities() {
  // Create ground body
  {
    auto ground = registry.create();
    registry.emplace<Transform>(ground, 512, 700);
    registry.emplace<Renderable>(ground, SDL_FColor{0.f, 1.f, 0.f, 1.f}, 1000, 50);
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
    registry.emplace<Renderable>(box, SDL_FColor{1.f, 0.f, 0.f, 1.f}, 40.0f, 40.0f);
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
    case SDL_EVENT_QUIT: running = false; break;
    case SDL_EVENT_KEY_DOWN:
      if (event.key.key == SDLK_ESCAPE) {
        running = false;
      }
      break;
    }
  }
}

void Application::Update() {
  b2World_Step(worldId, TIME_STEP, 4);

  auto view = registry.view<Rigidbody, Transform>();
  for (auto entity : view) {
    auto &rigidBody = view.get<Rigidbody>(entity);
    auto &transform = view.get<Transform>(entity);

    b2Vec2 position = b2Body_GetPosition(rigidBody.bodyId);
    b2Rot angle = b2Body_GetRotation(rigidBody.bodyId);
    f32 angleDegrees = b2Rot_GetAngle(angle);

    transform.x = position.x * PIXELS_PER_METER;
    transform.y = position.y * PIXELS_PER_METER;
    transform.rotation = angleDegrees;
  }
}

void Application::Render() {
  SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
  SDL_RenderClear(renderer);

  auto view = registry.view<Transform, Renderable>();
  for (auto entity : view) {
    const auto &transform = view.get<Transform>(entity);
    const auto &renderable = view.get<Renderable>(entity);

    SDL_SetRenderDrawColorFloat(renderer, renderable.color.r, renderable.color.g, renderable.color.b, renderable.color.a);
    SDL_FRect rect{transform.x - renderable.width / 2.0f, transform.y - renderable.height / 2.0f, renderable.width, renderable.height};

    SDL_RenderFillRect(renderer, &rect);
  }

  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Stats");
  ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
  ImGui::Text("ESC to exit");
  ImGui::End();

  ImGui::Render();
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
  SDL_RenderPresent(renderer);
}

void Application::Run() {
  while (running) {
    HandleEvents();
    Update();
    Render();
    SDL_Delay(6);
  }
}

void Application::Cleanup() {
  if (worldId.index1 != 0) {
    b2DestroyWorld(worldId);
  }

  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  if (renderer) {
    SDL_DestroyRenderer(renderer);
  }

  if (window) {
    SDL_DestroyWindow(window);
  }

  SDL_Quit();
}
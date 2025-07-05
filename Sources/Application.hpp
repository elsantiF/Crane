#pragma once

#include <SDL3/SDL.h>
#include <box2d/box2d.h>
#include <entt/entt.hpp>

class Application {
public:
  Application() : window(nullptr), renderer(nullptr), running(false) {}
  ~Application() { Cleanup(); }

  bool Initialize();
  void Run();

private:
  bool InitializeSDL();
  void InitializeImGui();
  void InitializeBox2D();
  void InitializeEntities();
  void HandleEvents();
  void Update();
  void Render();
  void Cleanup();

private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  b2WorldId worldId;
  entt::registry registry;
  bool running;

  const float PIXELS_PER_METER = 30.0f;
  const float TIME_STEP = 1.0f / 165.0f;
};
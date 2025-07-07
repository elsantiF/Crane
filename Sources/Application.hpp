#pragma once

#include "Core/Types.hpp"
#include "Core/World.hpp"
#include <box2d/box2d.h>
#include <entt/entt.hpp>
#include <SDL3/SDL.h>

class Application {
public:
  Application() : window(nullptr), renderer(nullptr), running(false) {}
  ~Application() { Cleanup(); }

  bool Initialize();
  void Run();

private:
  bool InitializeSDL();
  void InitializeImGui();
  void InitializeEntities();
  void HandleEvents();
  void Update(f64 deltaTime);
  void Render();
  void Cleanup();

private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  World world;
  bool running;

  f64 m_DeltaTime = 0.0;
  f64 m_LastTime = 0.0;

  const f32 PIXELS_PER_METER = 30.0f;
};
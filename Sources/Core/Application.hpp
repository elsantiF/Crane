#pragma once

#include "Core/Types.hpp"
#include "Graphics/IRenderer.hpp"
#include "Systems/RenderingSystem.hpp"
#include "World/World.hpp"
#include <box2d/box2d.h>
#include <entt/entt.hpp>
#include <SDL3/SDL.h>

namespace Crane::Core {
  class Application {
  public:
    Application() : m_Window(nullptr), m_Renderer(nullptr), m_Running(false) {}
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
    SDL_Window *m_Window;
    Scope<Graphics::IRenderer> m_Renderer;
    World::World m_World;
    bool m_Running;

    f64 m_DeltaTime = 0.0;

    const f32 PIXELS_PER_METER = 30.0f;
    const f32 DELTA_LOW_CAP = 0.0167f; // 60 FPS, keep below it
    const f32 DELTA_HIGH_CAP = 0.1f;   // 10 FPS, keep above it
  };
}
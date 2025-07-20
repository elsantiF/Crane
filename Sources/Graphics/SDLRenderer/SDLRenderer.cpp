#include "SDLRenderer.hpp"
#include "Core/Profiler.hpp"
#include <cmath>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

namespace Crane::Graphics::SDLRenderer {
  bool SDLRenderer::Initialize() {
    PROFILE_SCOPE();
    m_Renderer = SDL_CreateRenderer(m_Window, NULL);
    if (!m_Renderer) {
      return false;
    }

    // TODO: Make VSync optional
    SDL_SetRenderVSync(m_Renderer, 1);

    return true;
  }

  void SDLRenderer::Shutdown() {
    if (m_Renderer) {
      SDL_DestroyRenderer(m_Renderer);
    }
  }

  void SDLRenderer::InitializeImGui() {
    ImGui_ImplSDL3_InitForSDLRenderer(m_Window, m_Renderer);
    ImGui_ImplSDLRenderer3_Init(m_Renderer);
  }

  void SDLRenderer::ShutdownImGui() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
  }

  void SDLRenderer::BeginImGuiFrame() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
  }

  void SDLRenderer::EndImGuiFrame() {
    ImGui::End();
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_Renderer);
  }

  void SDLRenderer::BeginFrame() {
    // Nothing needed
    return;
  }

  void SDLRenderer::EndFrame() {
    // Nothing needed
    return;
  }

  void SDLRenderer::Clear(const Color &color) {
    PROFILE_SCOPE();
    SDL_SetRenderDrawColorFloat(m_Renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_Renderer);
  }

  void SDLRenderer::Present() {
    PROFILE_SCOPE();
    SDL_RenderPresent(m_Renderer);
  }

  void SDLRenderer::DrawRect(const Rect &rect, const Color &color, f32 rotation) {
    PROFILE_SCOPE();

    f32 centerX = rect.x + rect.width / 2.0f;
    f32 centerY = rect.y + rect.height / 2.0f;

    f32 cosAngle = cosf(rotation);
    f32 sinAngle = sinf(rotation);

    SDL_FPoint points[4];
    f32 halfWidth = rect.width / 2.0f;
    f32 halfHeight = rect.height / 2.0f;

    points[0] = {-halfWidth, -halfHeight};
    points[1] = {halfWidth, -halfHeight};
    points[2] = {halfWidth, halfHeight};
    points[3] = {-halfWidth, halfHeight};

    for (i32 i = 0; i < 4; ++i) {
      f32 x = points[i].x * cosAngle - points[i].y * sinAngle;
      f32 y = points[i].x * sinAngle + points[i].y * cosAngle;
      points[i] = {x + centerX, y + centerY};
    }

    static Color currentColor{-1.0f, -1.0f, -1.0f, -1.0f};
    if (currentColor != color) {
      SDL_SetRenderDrawColorFloat(m_Renderer, color.r, color.g, color.b, color.a);
      currentColor = color;
    }

    SDL_Vertex vertices[4];
    for (i32 i = 0; i < 4; ++i) {
      vertices[i].position = points[i];
      vertices[i].color = color;
    }

    i32 indices[6] = {0, 1, 2, 2, 3, 0};

    SDL_RenderGeometry(m_Renderer, nullptr, vertices, 4, indices, 6);
  }

  void SDLRenderer::DrawLine(const Math::Vec2f &start, const Math::Vec2f &end, const Color &color) {
    PROFILE_SCOPE();
    static Color currentColor{-1.0f, -1.0f, -1.0f, -1.0f};

    if (currentColor != color) {
      SDL_SetRenderDrawColorFloat(m_Renderer, color.r, color.g, color.b, color.a);
      currentColor = color;
    }

    SDL_FPoint points[2];
    points[0] = {start.x, start.y};
    points[1] = {end.x, end.y};
    SDL_RenderLines(m_Renderer, points, 2);
  }
}
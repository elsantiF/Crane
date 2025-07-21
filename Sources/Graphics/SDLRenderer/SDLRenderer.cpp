#include "SDLRenderer.hpp"
#include "Core/Profiler.hpp"
#include <cmath>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <numbers>

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

  void SDLRenderer::SetFillColor(const Color &color) {
    PROFILE_SCOPE();
    if (m_FillColor != color) {
      SDL_SetRenderDrawColorFloat(m_Renderer, color.r, color.g, color.b, color.a);
      m_FillColor = color;
    }
  }

  void SDLRenderer::DrawPoint(const Math::Vec2f &point) {
    PROFILE_SCOPE();

    SDL_FPoint points[4];
    points[0] = {point.x - 3.f, point.y - 3.f};
    points[1] = {point.x + 3.f, point.y - 3.f};
    points[2] = {point.x + 3.f, point.y + 3.f};
    points[3] = {point.x - 3.f, point.y + 3.f};

    SDL_Vertex vertices[4];
    for (i32 i = 0; i < 4; ++i) {
      vertices[i].position = points[i];
      vertices[i].color = m_FillColor;
    }

    i32 indices[6] = {0, 1, 2, 2, 3, 0};

    SDL_RenderGeometry(m_Renderer, nullptr, vertices, 4, indices, 6);
  }

  void SDLRenderer::DrawLine(const Math::Vec2f &start, const Math::Vec2f &end) {
    PROFILE_SCOPE();

    SDL_FPoint points[2];
    points[0] = {start.x, start.y};
    points[1] = {end.x, end.y};
    SDL_RenderLines(m_Renderer, points, 2);
  }

  void SDLRenderer::DrawRect(const Rect &rect, f32 rotation) {
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

    SDL_Vertex vertices[4];
    for (i32 i = 0; i < 4; ++i) {
      vertices[i].position = points[i];
      vertices[i].color = m_FillColor;
    }

    i32 indices[6] = {0, 1, 2, 2, 3, 0};

    SDL_RenderGeometry(m_Renderer, nullptr, vertices, 4, indices, 6);
  }

  void SDLRenderer::DrawCircle(const Math::Vec2f &center, f32 radius) {
    PROFILE_SCOPE();

    const i32 segments = 32;
    SDL_Vertex vertices[segments + 1];
    i32 indices[segments * 3];

    vertices[0].position = {center.x, center.y};
    vertices[0].color = m_FillColor;

    for (i32 i = 0; i < segments; ++i) {
      f32 angle = 2.0f * std::numbers::pi_v<f32> * i / segments;
      vertices[i + 1].position = {center.x + radius * cosf(angle), center.y + radius * sinf(angle)};
      vertices[i + 1].color = m_FillColor;
    }

    for (i32 i = 0; i < segments; ++i) {
      indices[i * 3] = 0;
      indices[i * 3 + 1] = i + 1;
      indices[i * 3 + 2] = (i + 1) % segments + 1;
    }

    SDL_RenderGeometry(m_Renderer, nullptr, vertices, segments + 1, indices, segments * 3);
  }
}
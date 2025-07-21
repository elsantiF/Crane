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

  // TODO: extract renderable to its own class, somthing like a VertexBuffer
  void SDLRenderer::DrawRenderable(const Components::Renderable &renderable, const Math::Transform &transform) {
    PROFILE_SCOPE();

    if (renderable.vertexCount == 0 || renderable.indexCount == 0) {
      return;
    }

    Vector<SDL_Vertex> vertices(renderable.vertexCount);
    f32 angle = transform.rotation;
    f32 cosAngle = std::cos(angle);
    f32 sinAngle = std::sin(angle);
    for (i32 i = 0; i < renderable.vertexCount; ++i) {
      f32 rotatedX = renderable.vertices[i].x * cosAngle - renderable.vertices[i].y * sinAngle;
      f32 rotatedY = renderable.vertices[i].x * sinAngle + renderable.vertices[i].y * cosAngle;
      f32 vertX = rotatedX + transform.position.x;
      f32 vertY = rotatedY + transform.position.y;
      vertices[i].position = {vertX, vertY};
      vertices[i].color = renderable.color;
    }

    Vector<i32> indices(renderable.indexCount);
    for (i32 i = 0; i < renderable.indexCount; ++i) {
      indices[i] = renderable.indices[i];
    }

    SDL_RenderGeometry(m_Renderer, nullptr, vertices.data(), renderable.vertexCount, indices.data(), renderable.indexCount);
  }
}
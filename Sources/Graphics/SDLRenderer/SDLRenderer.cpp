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

  u32 SDLRenderer::LoadVertexData(const SVertex2List &vertices) {
    PROFILE_SCOPE();
    u32 vertexDataId = static_cast<u32>(m_Context.vertexData.size()) + 1;
    Vector<SDL_Vertex> sdlVertices(vertices.size());

    for (size_t i = 0; i < vertices.size(); ++i) {
      sdlVertices[i].position = {vertices[i].position.x, vertices[i].position.y};
      sdlVertices[i].color = {vertices[i].color.r, vertices[i].color.g, vertices[i].color.b, vertices[i].color.a};
      sdlVertices[i].tex_coord = {vertices[i].uv.x, vertices[i].uv.y};
    }

    m_Context.vertexData[vertexDataId] = sdlVertices;
    return vertexDataId;
  }

  void SDLRenderer::UnloadVertexData(u32 vertexDataId) {
    PROFILE_SCOPE();
    if (m_Context.activeVertexDataId == vertexDataId) {
      m_Context.activeVertexDataId = 0;
      m_Context.activeVertexData = nullptr;
    }

    auto it = m_Context.vertexData.find(vertexDataId);
    if (it != m_Context.vertexData.end()) {
      m_Context.vertexData.erase(it);
    }
  }

  u32 SDLRenderer::LoadIndexData(const IndexList &indices) {
    PROFILE_SCOPE();
    u32 indexDataId = static_cast<u32>(m_Context.indexData.size()) + 1;
    m_Context.indexData[indexDataId] = indices;
    return indexDataId;
  }

  void SDLRenderer::UnloadIndexData(u32 indexDataId) {
    PROFILE_SCOPE();
    if (m_Context.activeIndexDataId == indexDataId) {
      m_Context.activeIndexDataId = 0;
      m_Context.activeIndexData = nullptr;
    }

    auto it = m_Context.indexData.find(indexDataId);
    if (it != m_Context.indexData.end()) {
      m_Context.indexData.erase(it);
    }
  }

  u32 SDLRenderer::LoadTexture(const Texture &texture) {
    PROFILE_SCOPE();
    u32 textureId = static_cast<u32>(m_Context.textures.size()) + 1;
    SDL_Texture *sdlTexture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, texture.width, texture.height);

    if (!sdlTexture) {
      return 0;
    }

    SDL_UpdateTexture(sdlTexture, NULL, texture.data.data(), texture.width * 4);

    m_Context.textures[textureId] = sdlTexture;
    return textureId;
  }

  void SDLRenderer::UnloadTexture(u32 textureId) {
    PROFILE_SCOPE();
    if (m_Context.activeTextureId == textureId) {
      m_Context.activeTextureId = 0;
      m_Context.activeTexture = nullptr;
    }

    auto it = m_Context.textures.find(textureId);
    if (it != m_Context.textures.end()) {
      SDL_DestroyTexture(it->second);
      m_Context.textures.erase(it);
    }
  }

  void SDLRenderer::SetFillColor(const Color &color) {
    PROFILE_SCOPE();
    if (m_Context.fillColor != color) {
      SDL_SetRenderDrawColorFloat(m_Renderer, color.r, color.g, color.b, color.a);
      m_Context.fillColor = color;
    }
  }

  void SDLRenderer::SetVertexData(u32 vertexDataId) {
    PROFILE_SCOPE();
    if (m_Context.activeVertexDataId == vertexDataId) {
      return;
    }

    auto it = m_Context.vertexData.find(vertexDataId);
    if (it != m_Context.vertexData.end()) {
      m_Context.activeVertexDataId = vertexDataId;
      m_Context.activeVertexData = &it->second;
    }
  }

  void SDLRenderer::SetIndexData(u32 indexDataId) {
    PROFILE_SCOPE();
    if (m_Context.activeIndexDataId == indexDataId) {
      return;
    }

    auto it = m_Context.indexData.find(indexDataId);
    if (it != m_Context.indexData.end()) {
      m_Context.activeIndexDataId = indexDataId;
      m_Context.activeIndexData = &it->second;
    }
  }

  void SDLRenderer::SetTexture(u32 textureId) {
    PROFILE_SCOPE();
    if (m_Context.activeTextureId == textureId) {
      return;
    }

    if (textureId == 0) {
      m_Context.activeTextureId = 0;
      m_Context.activeTexture = nullptr;
      return;
    }

    auto it = m_Context.textures.find(textureId);
    if (it != m_Context.textures.end()) {
      m_Context.activeTextureId = textureId;
      m_Context.activeTexture = it->second;
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
      vertices[i].color = m_Context.fillColor;
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
  void SDLRenderer::DrawRenderable(const Math::Transform &transform) {
    PROFILE_SCOPE();

    if (m_Context.activeIndexDataId == 0 || m_Context.activeVertexDataId == 0 || m_Context.activeVertexData == nullptr ||
        m_Context.activeIndexData == nullptr) {
      return;
    }

    Vector<SDL_Vertex> &vertices = *m_Context.activeVertexData;
    IndexList &indices = *m_Context.activeIndexData;

    Vector<SDL_Vertex> transformedVertices = vertices;

    f32 angle = transform.rotation;
    f32 cosAngle = std::cos(angle);
    f32 sinAngle = std::sin(angle);
    for (u32 i = 0; i < transformedVertices.size(); ++i) {
      f32 rotatedX = transformedVertices[i].position.x * cosAngle - transformedVertices[i].position.y * sinAngle;
      f32 rotatedY = transformedVertices[i].position.x * sinAngle + transformedVertices[i].position.y * cosAngle;
      f32 vertX = rotatedX + transform.position.x;
      f32 vertY = rotatedY + transform.position.y;
      transformedVertices[i].position = {vertX, vertY};
    }

    SDL_RenderGeometry(m_Renderer, m_Context.activeTexture, transformedVertices.data(), static_cast<u32>(transformedVertices.size()), indices.data(),
                       static_cast<u32>(indices.size()));
  }
}
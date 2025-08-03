#include "SDLRenderer.hpp"
#include "Base/Assert.hpp"
#include "Base/Logger.hpp"
#include "Base/Profiler.hpp"
#include <cmath>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL_render.h>

namespace Crane::Graphics::SDLRenderer {
  void SDLRenderer::Initialize() {
    PROFILE_SCOPE();
    m_Renderer = SDL_CreateRenderer(m_Window, NULL);
    if (!m_Renderer) {
      Logger::Error("Failed to create SDL Renderer: {}", SDL_GetError());
      Assert::Crash("Failed to create SDL Renderer");
    }

    // TODO: Make VSync optional
    SDL_SetRenderVSync(m_Renderer, 1);
    Logger::Info("SDLRenderer initialized successfully");

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForSDLRenderer(m_Window, m_Renderer);
    ImGui_ImplSDLRenderer3_Init(m_Renderer);
    Logger::Info("ImGui initialized for SDLRenderer");
  }

  void SDLRenderer::Shutdown() {
    if (m_Renderer) {
      Logger::Info("Shutting down SDLRenderer");
      SDL_DestroyRenderer(m_Renderer);
      m_Renderer = nullptr;
    }

    if (!m_Renderer) {
      Logger::Info("SDLRenderer has been shut down");
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    Logger::Info("ImGui shutdown for SDLRenderer");
  }

  void SDLRenderer::BeginImGuiFrame() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
  }

  void SDLRenderer::EndImGuiFrame() {
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

  Id SDLRenderer::LoadMesh(const Mesh &mesh) {
    PROFILE_SCOPE();
    Id meshId = static_cast<Id>(m_Context.meshes.size()) + 1;
    Vector<SDL_Vertex> sdlVertices(mesh.vertices.size());

    for (size_t i = 0; i < mesh.vertices.size(); ++i) {
      sdlVertices[i].position = {mesh.vertices[i].position.x, mesh.vertices[i].position.y};
      sdlVertices[i].color = {mesh.vertices[i].color.r, mesh.vertices[i].color.g, mesh.vertices[i].color.b, mesh.vertices[i].color.a};
      sdlVertices[i].tex_coord = {mesh.vertices[i].uv.x, mesh.vertices[i].uv.y};
    }

    m_Context.meshes[meshId] = {sdlVertices, mesh.indices};
    Logger::Info("Loaded mesh with ID: {}", meshId);
    return meshId;
  }

  void SDLRenderer::UnloadMesh(Id meshId) {
    PROFILE_SCOPE();
    if (m_Context.activeMeshId == meshId) {
      m_Context.activeMeshId = 0;
      m_Context.activeMesh = nullptr;
    }

    auto it = m_Context.meshes.find(meshId);
    if (it != m_Context.meshes.end()) {
      m_Context.meshes.erase(it);
      Logger::Info("Unloaded mesh with ID: {}", meshId);
    } else {
      Logger::Error("Failed to unload mesh with ID: {} - not found", meshId);
    }
  }

  Id SDLRenderer::LoadTexture(Resources::Resource<Texture> textureResource) {
    PROFILE_SCOPE();
    const Texture texture = *textureResource;
    Id textureId = static_cast<Id>(m_Context.textures.size()) + 1;
    SDL_Texture *sdlTexture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, texture.width, texture.height);

    if (!sdlTexture) {
      Logger::Error("Failed to create SDL texture: {}", SDL_GetError());
      return 0;
    }

    SDL_UpdateTexture(sdlTexture, NULL, texture.data.data(), texture.width * 4);

    m_Context.textures[textureId] = sdlTexture;
    Logger::Info("Loaded texture with ID: {}", textureId);
    return textureId;
  }

  void SDLRenderer::UnloadTexture(Id textureId) {
    PROFILE_SCOPE();
    if (m_Context.activeTextureId == textureId) {
      m_Context.activeTextureId = 0;
      m_Context.activeTexture = nullptr;
    }

    auto it = m_Context.textures.find(textureId);
    if (it != m_Context.textures.end()) {
      SDL_DestroyTexture(it->second);
      m_Context.textures.erase(it);
      Logger::Info("Unloaded texture with ID: {}", textureId);
    } else {
      Logger::Error("Failed to unload texture with ID: {} - not found", textureId);
    }
  }

  // TODO: This should check if the color is different before setting it
  //       to avoid unnecessary state changes, but is kinda buggy.
  void SDLRenderer::SetFillColor(const Color &color) {
    PROFILE_SCOPE();
    SDL_SetRenderDrawColorFloat(m_Renderer, color.r, color.g, color.b, color.a);
    m_Context.fillColor = color;
  }

  void SDLRenderer::SetMesh(Id meshId) {
    PROFILE_SCOPE();
    if (m_Context.activeMeshId == meshId) {
      return;
    }

    auto it = m_Context.meshes.find(meshId);
    if (it != m_Context.meshes.end()) {
      m_Context.activeMeshId = meshId;
      m_Context.activeMesh = &it->second;
    }
  }

  void SDLRenderer::SetTexture(Id textureId) {
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
      SDL_FColor fillColor = {m_Context.fillColor.r, m_Context.fillColor.g, m_Context.fillColor.b, m_Context.fillColor.a};
      vertices[i].color = fillColor;
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

  void SDLRenderer::DrawRenderable(const Math::Transform &transform) {
    PROFILE_SCOPE();

    if (m_Context.activeMeshId == 0 || m_Context.activeMesh == nullptr) {
      return;
    }

    Vector<SDL_Vertex> &vertices = m_Context.activeMesh->first;
    IndexList &indices = m_Context.activeMesh->second;

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
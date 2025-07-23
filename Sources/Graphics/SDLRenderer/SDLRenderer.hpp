#pragma once

#include "Core/Types.hpp"
#include "Graphics/IRenderer.hpp"
#include <SDL3/SDL_render.h>

namespace Crane::Graphics::SDLRenderer {
  class SDLRenderer : public IRenderer {
  public:
    SDLRenderer(SDL_Window *window) : m_Renderer(nullptr), m_Window(window) {}
    ~SDLRenderer() override = default;

    bool Initialize() override;
    void Shutdown() override;

    void InitializeImGui() override;
    void ShutdownImGui() override;
    void BeginImGuiFrame() override;
    void EndImGuiFrame() override;

    void BeginFrame() override;
    void EndFrame() override;
    void Clear(const Color &color) override;
    void Present() override;

    u32 LoadVertexData(const SVertex2List &vertices) override;
    void UnloadVertexData(u32 vertexDataId) override;

    u32 LoadIndexData(const IndexList &indices) override;
    void UnloadIndexData(u32 indexDataId) override;

    void SetFillColor(const Color &color) override;
    void SetVertexData(u32 vertexDataId) override;
    void SetIndexData(u32 indexDataId) override;

    void DrawPoint(const Math::Vec2f &point) override;
    void DrawLine(const Math::Vec2f &start, const Math::Vec2f &end) override;
    void DrawRenderable(const Math::Transform &transform) override;

    String GetName() const override {
      return "SDLRenderer";
    }

  private:
    SDL_Renderer *m_Renderer;
    SDL_Window *m_Window; // Perhaps we should not store this here

    struct SDLRendererContext {
      Color fillColor = {-1.0f, -1.0f, -1.0f, -1.0f};

      Map<u32, Vector<SDL_Vertex>> vertexData;
      Map<u32, IndexList> indexData;

      u32 activeVertexDataId = 0;
      Vector<SDL_Vertex> *activeVertexData = nullptr;
      u32 activeIndexDataId = 0;
      IndexList *activeIndexData = nullptr;
    } m_Context;
  };
}
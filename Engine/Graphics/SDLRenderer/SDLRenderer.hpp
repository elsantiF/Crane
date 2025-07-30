#pragma once

#include "Base/Types.hpp"
#include "Graphics/IRenderer.hpp"

struct SDL_Renderer;
struct SDL_Window;

namespace Crane::Graphics::SDLRenderer {
  class SDLRenderer : public IRenderer {
  public:
    SDLRenderer(SDL_Window *window) : m_Renderer(nullptr), m_Window(window) {}
    ~SDLRenderer() override = default;

    bool Initialize() override;
    void Shutdown() override;

    void BeginImGuiFrame() override;
    void EndImGuiFrame() override;

    void BeginFrame() override;
    void EndFrame() override;
    void Clear(const Color &color) override;
    void Present() override;

    Id LoadVertexData(const SVertex2List &vertices) override;
    void UnloadVertexData(Id vertexDataId) override;

    Id LoadIndexData(const IndexList &indices) override;
    void UnloadIndexData(Id indexDataId) override;

    Id LoadTexture(const Texture &texture) override;
    void UnloadTexture(Id textureId) override;

    void SetFillColor(const Color &color) override;
    void SetVertexData(Id vertexDataId) override;
    void SetIndexData(Id indexDataId) override;
    void SetTexture(Id textureId) override;

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

      Map<Id, Vector<SDL_Vertex>> vertexData;
      Map<Id, IndexList> indexData;
      Map<Id, SDL_Texture *> textures;

      Id activeVertexDataId = 0;
      Vector<SDL_Vertex> *activeVertexData = nullptr;
      Id activeIndexDataId = 0;
      IndexList *activeIndexData = nullptr;
      Id activeTextureId = 0;
      SDL_Texture *activeTexture = nullptr;
    } m_Context;
  };
}
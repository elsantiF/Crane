#pragma once

#include "Base/Types.hpp"
#include "Graphics/Renderer/IRenderer.hpp"

struct SDL_Renderer;
struct SDL_Window;
struct SDL_Vertex;
struct SDL_Texture;

namespace Crane::Graphics::SDLRenderer {
  class SDLRenderer : public IRenderer {
    using SDLMesh = Pair<Vector<SDL_Vertex>, IndexList>;

  public:
    SDLRenderer(SDL_Window *window) : m_Renderer(nullptr), m_Window(window) {}
    ~SDLRenderer() override = default;

    void Initialize() override;
    void Shutdown() override;

    void BeginImGuiFrame() override;
    void EndImGuiFrame() override;

    void BeginFrame() override;
    void EndFrame() override;
    void Clear(const Color &color) override;
    void Present() override;

    Id LoadMesh(const Mesh &mesh) override;
    void UnloadMesh(Id meshId) override;

    Id LoadTexture(Resources::Resource<Texture> textureResource) override;
    void UnloadTexture(Id textureId) override;

    void SetFillColor(const Color &color) override;
    void SetMesh(Id meshId) override;
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

      Map<Id, SDLMesh> meshes;
      Map<Id, SDL_Texture *> textures;

      Id activeMeshId = 0;
      SDLMesh *activeMesh = nullptr;
      Id activeTextureId = 0;
      SDL_Texture *activeTexture = nullptr;
    } m_Context;
  };
}
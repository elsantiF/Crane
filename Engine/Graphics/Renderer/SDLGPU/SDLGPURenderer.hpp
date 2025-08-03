#pragma once

#include "Base/Types.hpp"
#include "Graphics/Renderer/IRenderer.hpp"
#include <SDL3/SDL_gpu.h>

namespace Crane::Graphics::SDLGPURenderer {
  class SDLGPURenderer : public IRenderer {
  public:
    SDLGPURenderer(SDL_Window *window) : m_Window(window) {}
    ~SDLGPURenderer() override = default;

    void Initialize() override;
    void Shutdown() override;

    void BeginImGuiFrame() override;
    void EndImGuiFrame() override;

    void BeginFrame() override {};
    void EndFrame() override {};
    void Clear(const Color &color) override {};
    void Present() override {};

    Id LoadMesh(const Mesh &mesh) override {
      return 0;
    };
    void UnloadMesh(Id meshId) override {};

    Id LoadTexture(Resources::Resource<Texture> textureResource) override {
      return 0;
    };
    void UnloadTexture(Id textureId) override {};

    void SetFillColor(const Color &color) override {};
    void SetMesh(Id meshId) override {};
    void SetTexture(Id textureId) override {};

    void DrawPoint(const Math::Vec2f &point) override {};
    void DrawLine(const Math::Vec2f &start, const Math::Vec2f &end) override {};
    void DrawRenderable(const Math::Transform &transform) override {};

    String GetName() const override {
      return "SDLGPURenderer";
    }

  private:
    SDL_Window *m_Window;

    struct {
      SDL_GPUDevice *gpuDevice = nullptr;
    } m_Context;
  };
}
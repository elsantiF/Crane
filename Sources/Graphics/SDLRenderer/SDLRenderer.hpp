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

    void DrawRect(const Rect &rect, const Color &color) override;
    String GetName() const override {
      return "SDLRenderer";
    }

  private:
    SDL_Renderer *m_Renderer;
    SDL_Window *m_Window; // Perhaps we should not store this here
  };
}
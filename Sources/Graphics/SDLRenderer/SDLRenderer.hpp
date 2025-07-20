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

    void DrawPoint(const Math::Vec2f &point, const Color &color) override;
    void DrawLine(const Math::Vec2f &start, const Math::Vec2f &end, const Color &color) override;
    void DrawRect(const Rect &rect, const Color &color, f32 rotation) override;
    void DrawCircle(const Math::Vec2f &center, f32 radius, const Color &color) override;

    String GetName() const override {
      return "SDLRenderer";
    }

  private:
    SDL_Renderer *m_Renderer;
    SDL_Window *m_Window; // Perhaps we should not store this here
  };
}
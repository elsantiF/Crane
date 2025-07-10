#include "SDLRenderer.hpp"
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

namespace Crane::Graphics::SDLRenderer {
  bool SDLRenderer::Initialize() {
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
  }

  void SDLRenderer::EndImGuiFrame() { ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_Renderer); }

  void SDLRenderer::BeginFrame() {
    // Nothing needed
    return;
  }

  void SDLRenderer::EndFrame() {
    // Nothing needed
    return;
  }

  void SDLRenderer::Clear(const Color &color) {
    SDL_SetRenderDrawColor(m_Renderer, color.GetR(), color.GetG(), color.GetB(), color.GetA());
    SDL_RenderClear(m_Renderer);
  }

  void SDLRenderer::Present() { SDL_RenderPresent(m_Renderer); }

  void SDLRenderer::DrawRect(const Rect &rect, const Color &color) {
    SDL_SetRenderDrawColor(m_Renderer, color.GetR(), color.GetG(), color.GetB(), color.GetA());

    SDL_FRect sdlRect = static_cast<SDL_FRect>(rect);
    SDL_RenderFillRect(m_Renderer, &sdlRect);
  }
}
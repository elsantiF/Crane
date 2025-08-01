#include "SDLWindow.hpp"
#include "Base/Assert.hpp"
#include <SDL3/SDL_video.h>

namespace Crane::Graphics {
  SDLWindow::SDLWindow(StringView title, u32 width, u32 height) {
    m_Window = SDL_CreateWindow(title.data(), width, height, 0);
    if (!m_Window) {
      Assert::Crash("Failed to create SDL window: " + String(SDL_GetError()));
    }
  }

  SDLWindow::~SDLWindow() {
    if (m_Window) {
      SDL_DestroyWindow(m_Window);
      m_Window = nullptr;
    }
  }
}
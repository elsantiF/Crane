#pragma once

#include "Base/Types.hpp"

class SDL_Window;

namespace Crane::Graphics {
  class SDLWindow {
  public:
    SDLWindow(StringView title, u32 width, u32 height);
    ~SDLWindow();

    SDL_Window *GetHandle() const {
      return m_Window;
    }

  private:
    SDL_Window *m_Window;
  };
}
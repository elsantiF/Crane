#pragma once

#include "Core/Types.hpp"
#include <SDL3/SDL_rect.h>

class Rect {
public:
  Rect(f32 x = 0.0f, f32 y = 0.0f, f32 width = 0.0f, f32 height = 0.0f) : m_X(x), m_Y(y), m_Width(width), m_Height(height) {}

  f32 GetX() const { return m_X; }
  f32 GetY() const { return m_Y; }
  f32 GetWidth() const { return m_Width; }
  f32 GetHeight() const { return m_Height; }

  void SetX(f32 x) { m_X = x; }
  void SetY(f32 y) { m_Y = y; }
  void SetWidth(f32 width) { m_Width = width; }
  void SetHeight(f32 height) { m_Height = height; }

  SDL_FRect ToSDLRect() const { return SDL_FRect{m_X, m_Y, m_Width, m_Height}; }

private:
  f32 m_X, m_Y;
  f32 m_Width, m_Height;
};
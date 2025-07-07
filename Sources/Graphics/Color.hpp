#pragma once

#include "Core/Types.hpp"
#include <SDL3/SDL_render.h>

class Color {
public:
  Color() : m_R(0), m_G(0), m_B(0), m_A(255) {}
  Color(u8 r, u8 g, u8 b, u8 a = 255) : m_R(r), m_G(g), m_B(b), m_A(a) {}

  u8 GetR() const { return m_R; }
  u8 GetG() const { return m_G; }
  u8 GetB() const { return m_B; }
  u8 GetA() const { return m_A; }

  void SetR(u8 r) { m_R = r; }
  void SetG(u8 g) { m_G = g; }
  void SetB(u8 b) { m_B = b; }
  void SetA(u8 a) { m_A = a; }

  Color FromSDLColor(const SDL_Color &color) { return Color(color.r, color.g, color.b, color.a); }
  SDL_Color ToSDLColor() const { return SDL_Color{m_R, m_G, m_B, m_A}; }

private:
  u8 m_R, m_G, m_B, m_A;
};
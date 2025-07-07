#pragma once

#include "Core/Types.hpp"
#include "Graphics/Color.hpp"

struct Renderable {
  Color color;
  f32 width, height;

  Renderable(Color color = {255, 255, 255, 255}, f32 width = 32.0f, f32 height = 32.0f) : color(color), width(width), height(height) {}
};
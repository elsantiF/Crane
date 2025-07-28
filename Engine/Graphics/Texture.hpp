#pragma once

#include "Core/Types.hpp"

namespace Crane::Graphics {
  struct Texture {
    u32 width;
    u32 height;
    Vector<u32> data; // Pixel data in RGBA format, 8 bits per channel, stored as u32 per pixel
  };
}
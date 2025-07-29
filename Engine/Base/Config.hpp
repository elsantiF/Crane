#pragma once

#include "Base/Math/Vector.hpp"
#include "Base/Types.hpp"
#include "Graphics/Color.hpp"

namespace Crane {
  namespace Core {
    constexpr f64 PHYSICS_TIMESTEP = 1.0 / 60.0;
    constexpr i32 WINDOW_WIDTH = 1600;
    constexpr i32 WINDOW_HEIGHT = 900;
  }

  namespace Physics {
    constexpr i32 PHYSICS_STEPS = 4;
  }

  namespace Colors {
    const Graphics::Color CLEAR_COLOR = {0.05f, 0.05f, 0.05f, 1.0f};
  }

  namespace Scene {
    constexpr f32 PIXELS_PER_METER = 30.0f;
    constexpr Math::Vec2f GRAVITY = {0.0f, 9.81f};
  }
}
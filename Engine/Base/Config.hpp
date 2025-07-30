#pragma once

#include "Base/Math/Vector.hpp"
#include "Base/Types.hpp"

namespace Crane {
  namespace Core {
    constexpr f64 PHYSICS_TIMESTEP = 1.0 / 60.0;
  }

  namespace Physics {
    constexpr i32 PHYSICS_STEPS = 4;
  }

  namespace Scene {
    constexpr f32 PIXELS_PER_METER = 30.0f;
  }
}
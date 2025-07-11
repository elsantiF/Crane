#pragma once

#include "BodyStructs.hpp"
#include "Core/Types.hpp"

namespace Crane::Physics {
  class PhysicsWorld;

  class PhysicsFactory {
  public:
    static b2BodyId CreateBoxBody(PhysicsWorld &world, BodyConfig config, f32 ppm);
  };
}
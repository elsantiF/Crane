#pragma once

#include "Core/Types.hpp"
#include <box2d/box2d.h>

namespace Crane::Physics {
  enum class BodyType { Static = b2_staticBody, Dynamic = b2_dynamicBody, Kinematic = b2_kinematicBody };

  struct BodyConfig {
    f32 x, y;
    f32 width, height;
    BodyType type;
  };
}
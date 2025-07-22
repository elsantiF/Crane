#pragma once

#include "Core/Types.hpp"
#include "Math/Vector.hpp"
#include <box2d/box2d.h>

namespace Crane::Physics {
  enum class BodyType { Static = b2_staticBody, Dynamic = b2_dynamicBody, Kinematic = b2_kinematicBody };

  struct BoxBodyConfig {
    Math::Vec2f center;
    Math::Vec2f dimensions;
    BodyType type;
  };

  struct CircleBodyConfig {
    Math::Vec2f center;
    f32 radius;
    BodyType type;
  };
}
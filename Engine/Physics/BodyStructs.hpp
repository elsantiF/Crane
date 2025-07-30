#pragma once

#include "Base/Math/Vector.hpp"
#include "Base/Types.hpp"

namespace Crane::Physics {
  enum class BodyType { Static = 0, Kinematic = 1, Dynamic = 2 };

  struct BaseBodyConfig {
    Math::Vec2f center;
    BodyType type;
  };

  struct BoxBodyConfig : public BaseBodyConfig {
    Math::Vec2f dimensions;

    BoxBodyConfig(Math::Vec2f center, Math::Vec2f dimensions, BodyType type) : BaseBodyConfig{center, type}, dimensions(dimensions) {}
  };

  struct CircleBodyConfig : public BaseBodyConfig {
    f32 radius;

    CircleBodyConfig(Math::Vec2f center, f32 radius, BodyType type) : BaseBodyConfig{center, type}, radius(radius) {}
  };
}
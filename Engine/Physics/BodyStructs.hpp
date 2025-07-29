#pragma once

#include "Base/Math/Vector.hpp"
#include "Base/Types.hpp"
#include <box2d/box2d.h>

namespace Crane::Physics {
  enum class BodyType { Static = b2_staticBody, Dynamic = b2_dynamicBody, Kinematic = b2_kinematicBody };

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
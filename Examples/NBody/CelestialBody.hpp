#pragma once

#include "Base/Math/Vector.hpp"
#include "Base/Types.hpp"

struct CelestialBody {
  f64 mass;
  f64 radius;
  Crane::Math::Vec2f velocity;
  Crane::Math::Vec2f acceleration;

  CelestialBody(f64 m = 0.0, f64 r = 0.0, Crane::Math::Vec2f v = {0.0f, 0.0f}, Crane::Math::Vec2f a = {0.0f, 0.0f})
      : mass(m), radius(r), velocity(v), acceleration(a) {}
};
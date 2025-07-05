#pragma once

struct Transform {
  float x, y;
  float rotation;
  float scaleX, scaleY;

  Transform(float x = 0.0f, float y = 0.0f, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f)
      : x(x), y(y), rotation(rotation), scaleX(scaleX), scaleY(scaleY) {}
};
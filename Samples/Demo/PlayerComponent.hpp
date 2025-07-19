#pragma once

struct PlayerComponent {
  PlayerComponent() = default;

  bool isMovingLeft = false;
  bool isMovingRight = false;
};
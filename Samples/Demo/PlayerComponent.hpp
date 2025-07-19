#pragma once

struct PlayerComponent {
  PlayerComponent() = default;

  bool isMovingLeft = false;
  bool isMovingRight = false;
  bool isJumping = false;
};
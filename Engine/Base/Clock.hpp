#pragma once

#include "Base/Types.hpp"
#include <chrono>

namespace Crane {
  class Clock {
  public:
    static f64 GetCurrentTime() {
      return std::chrono::duration_cast<std::chrono::duration<f64>>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }
  };
}
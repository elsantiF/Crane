#pragma once

#include "Base/Types.hpp"

namespace Crane {
  struct ApplicationInfo {
    String appName;

    struct {
      u32 major;
      u32 minor;
      u32 patch;
    } version;

    struct {
      u32 width;
      u32 height;
      bool vsync;
    } window;
  };
}

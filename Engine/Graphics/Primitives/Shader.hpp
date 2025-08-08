#pragma once

#include "Base/Types.hpp"

namespace Crane::Graphics {
  enum class ShaderType { Vertex, Fragment };

  struct ShaderCreateInfo {
    ShaderType type;
    const u8 *source;
    const u32 size;
    const String &entryPoint = "main";

    const u32 numSamplers = 0;
    const u32 numUniformBuffers = 0;
  };
}
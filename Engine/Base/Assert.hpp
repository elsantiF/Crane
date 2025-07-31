#pragma once

#include "Base/Logger.hpp"
#include "Base/Types.hpp"
#include <source_location>

namespace Crane {
  class Assert {
  public:
    static void Check(bool condition, const String &message, const std::source_location &location = std::source_location::current()) {
      if (!condition) {
        Logger::Error("Assertion Failed: {}\nLocation: {}:{}", message, location.file_name(), location.line());
      }
    }

    static void Crash(const String &message, const std::source_location &location = std::source_location::current()) {
      Logger::Error("Crash: {}\nLocation: {}:{}", message, location.file_name(), location.line());
      std::terminate();
    }
  };
}
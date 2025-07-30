#pragma once

#include "Base/Types.hpp"
#include <source_location>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace Crane {
  class Logger {
  public:
    static void Initialize() {
      spdlog::set_pattern("[%H:%M:%S] [%l] %v");
      auto fileSink = MakeRef<spdlog::sinks::basic_file_sink_mt>("logs/crane.log", true);
      auto consoleSink = MakeRef<spdlog::sinks::stdout_color_sink_mt>();
      m_Logger = MakeRef<spdlog::logger>("Crane", fileSink);
      m_Logger->sinks().push_back(consoleSink);
      spdlog::register_logger(m_Logger);
      m_Logger->set_level(spdlog::level::trace);
    }

    static Ref<spdlog::logger> GetLogger() {
      return m_Logger;
    }

    static void Info(const String &message, const std::source_location &location = std::source_location::current()) {
      m_Logger->info("[{}:{}] {}", location.file_name(), location.line(), message);
    }

    static void Error(const String &message, const std::source_location &location = std::source_location::current()) {
      m_Logger->error("[{}:{}] {}", location.file_name(), location.line(), message);
    }

    static void Shutdown() {
      if (m_Logger) {
        m_Logger->flush();
        spdlog::drop("Crane");
      }

      m_Logger.reset();
    }

  private:
    inline static Ref<spdlog::logger> m_Logger = nullptr;
  };
}
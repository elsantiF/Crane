set(CORE_SOURCES 
    "Config.hpp"
    "Logger.hpp"
    "Profiler.cpp"
    "Profiler.hpp"
    "Types.hpp"
)

add_library(CraneCore STATIC ${CORE_SOURCES})
target_include_directories(CraneCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneCore PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CraneCore PUBLIC Tracy::TracyClient spdlog::spdlog)

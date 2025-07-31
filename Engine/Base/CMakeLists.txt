set(BASE_SOURCES
    # Core files
    "Assert.hpp"
    "Config.hpp"
    "Logger.hpp"
    "Profiler.cpp"
    "Profiler.hpp"
    "Types.hpp"
    # Geometry
    "Geometry/Rect.hpp"
    # Math
    "Math/Matrix.hpp"
    "Math/Transform.hpp"
    "Math/Vector.hpp"
)

add_library(CraneBase STATIC ${BASE_SOURCES})
target_include_directories(CraneBase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneBase PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CraneBase PUBLIC Tracy::TracyClient spdlog::spdlog glm)
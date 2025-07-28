set(GEOMETRY_SOURCES 
    "Rect.hpp"
)

add_library(CraneGeometry INTERFACE)
target_sources(CraneGeometry INTERFACE ${GEOMETRY_SOURCES})
target_include_directories(CraneGeometry INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneGeometry INTERFACE "${CMAKE_SOURCE_DIR}/Sources")

set(PHYSICS_SOURCES 
    "BodyStructs.hpp"
)

add_library(CranePhysics INTERFACE ${PHYSICS_SOURCES})
target_include_directories(CranePhysics INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CranePhysics INTERFACE "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CranePhysics INTERFACE CraneMath CraneGraphics)
target_link_libraries(CranePhysics INTERFACE box2d)

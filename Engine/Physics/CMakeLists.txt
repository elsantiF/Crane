set(PHYSICS_SOURCES 
    "BodyStructs.hpp"
    "PhysicsSystem.cpp"
    "PhysicsSystem.hpp"
)

add_library(CranePhysics STATIC ${PHYSICS_SOURCES})
target_include_directories(CranePhysics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CranePhysics PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CranePhysics PUBLIC CraneCore CraneGraphics)
target_link_libraries(CranePhysics PUBLIC box2d)

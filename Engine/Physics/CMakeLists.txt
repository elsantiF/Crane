set(PHYSICS_SOURCES 
    "PhysicsSystem.cpp"
)

add_library(CranePhysics STATIC ${PHYSICS_SOURCES})
target_include_directories(CranePhysics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CranePhysics PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CranePhysics PUBLIC CraneCore box2d)

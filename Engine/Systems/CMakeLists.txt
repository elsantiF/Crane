set(SYSTEMS_SOURCES 
    "ISystem.hpp"
    "PhysicsSystem.hpp"
    "PhysicsSystem.cpp"
    "RenderingSystem.hpp"
    "RenderingSystem.cpp"
    "SystemManager.hpp"
    "SystemManager.cpp"
)

add_library(CraneSystems STATIC ${SYSTEMS_SOURCES})
target_include_directories(CraneSystems PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneSystems PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CraneSystems PUBLIC CraneMath CraneGraphics CranePhysics CraneWorld EnTT::EnTT)

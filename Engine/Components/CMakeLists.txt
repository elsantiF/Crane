set(COMPONENTS_SOURCES 
    "BoxCollider.hpp"
    "CircleCollider.hpp"
    "Renderable.hpp"
    "RigidBody.hpp"
    "Transform.hpp"
)

add_library(CraneComponents INTERFACE)
target_sources(CraneComponents INTERFACE ${COMPONENTS_SOURCES})
target_include_directories(CraneComponents INTERFACE "${CMAKE_SOURCE_DIR}/Engine")
target_include_directories(CraneComponents INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

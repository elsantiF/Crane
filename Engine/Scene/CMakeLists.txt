set(SCENE_SOURCES
    # Components
    "Components/BoxCollider.hpp"
    "Components/CircleCollider.hpp"
    "Components/Renderable.hpp"
    "Components/RigidBody.hpp"
    "Components/Transform.hpp"
    # Entity
    "Entity/Entity.hpp"
    "Entity/Entity.inl"
    # World
    "World/World.cpp"
    "World/World.hpp"
)

add_library(CraneScene STATIC ${SCENE_SOURCES})
target_include_directories(CraneScene PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneScene PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CraneScene PUBLIC CraneBase CraneGraphics CranePhysics CraneCore EnTT::EnTT)
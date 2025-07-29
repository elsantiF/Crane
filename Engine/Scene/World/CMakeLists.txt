set(WORLD_SOURCES 
    "Entity.hpp"
    "Entity.inl"
    "World.cpp"
    "World.hpp"
)

add_library(CraneWorld STATIC ${WORLD_SOURCES})
target_include_directories(CraneWorld PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneWorld PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CraneWorld PUBLIC CraneBase CraneGraphics CranePhysics CraneCore EnTT::EnTT)
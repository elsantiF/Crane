set(GRAPHICS_SOURCES 
    "SDLRenderer/SDLRenderer.cpp"
    "SDLRenderer/SDLRenderer.hpp"
    "Color.hpp"
    "IRenderer.hpp"
    "Texture.hpp"
    "Vertex.hpp"
)

add_library(CraneGraphics STATIC ${GRAPHICS_SOURCES})
target_include_directories(CraneGraphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneGraphics PUBLIC "${CMAKE_SOURCE_DIR}/Sources")
target_link_libraries(CraneGraphics PUBLIC CraneMath)
target_link_libraries(CraneGraphics PUBLIC imgui SDL3::SDL3)

set(GRAPHICS_SOURCES 
    "SDLRenderer/SDLRenderer.cpp"
    "Window/SDLWindow.cpp"
    "RenderPipeline.cpp"
    "TextureManager.cpp"
)

add_library(CraneGraphics STATIC ${GRAPHICS_SOURCES})
target_include_directories(CraneGraphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneGraphics PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CraneGraphics PUBLIC CraneCore imgui SDL3::SDL3 EnTT::EnTT stb)

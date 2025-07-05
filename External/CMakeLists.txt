add_subdirectory("box2d")
add_subdirectory("entt")
add_subdirectory("SDL")

file(GLOB IMGUI_SOURCES
    "imgui/*.cpp"
    "imgui/backends/imgui_impl_sdl3.cpp"
    "imgui/backends/imgui_impl_sdlrenderer3.cpp"
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC "imgui" "imgui/backends")
target_link_libraries(imgui PUBLIC SDL3::SDL3)
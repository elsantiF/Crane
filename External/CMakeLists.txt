add_subdirectory("box2d")
add_subdirectory("entt")

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "glm")

option(SDL_STATIC "" ON)
option(SDL_SHARED "" OFF)
option(SDL_TESTS "" OFF)
option(SDL_INSTALL "" OFF)
option(SDL_SAMPLES "" OFF)
option(SDL_CAMERA "" OFF)
option(SDL_HAPTIC "" OFF)
option(SDL_JOYSTICK "" OFF)
option(SDL_SENSOR "" OFF)
add_subdirectory("SDL")

option(SPDLOG_BUILD_EXAMPLES "" OFF)
option(SPDLOG_BUILD_TESTS "" OFF)
option(SPDLOG_BUILD_BENCH "" OFF)
add_subdirectory("spdlog")

add_subdirectory("tracy")

file(GLOB IMGUI_SOURCES
    "imgui/*.cpp"
    "imgui/backends/imgui_impl_sdl3.cpp"
    "imgui/backends/imgui_impl_sdlrenderer3.cpp"
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC "imgui" "imgui/backends")
target_link_libraries(imgui PUBLIC SDL3::SDL3)
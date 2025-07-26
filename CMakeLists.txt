cmake_minimum_required(VERSION 3.26)
project(Crane)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(ENABLE_PROFILER "Enable Tracy Profiler" OFF)

if (ENABLE_PROFILER)
    set(TRACY_ENABLE ON CACHE BOOL "" FORCE)
    set(TRACY_CALLSTACK ON CACHE BOOL "" FORCE)
else ()
    set(TRACY_ENABLE OFF CACHE BOOL "" FORCE)
    set(TRACY_CALLSTACK OFF CACHE BOOL "" FORCE)
endif ()

add_subdirectory("External")

add_library(Crane INTERFACE)
add_subdirectory("Sources")

target_link_libraries(Crane INTERFACE 
    CraneApplication
    CraneComponents
    CraneCore
    CraneEditor
    CraneEvents
    CraneGeometry
    CraneGraphics
    CraneMath
    CranePhysics
    CraneSystems
    CraneWorld
)
target_include_directories(Crane INTERFACE "Sources")
target_include_directories(Crane INTERFACE "External/entt/src")
target_link_libraries(Crane INTERFACE imgui SDL3::SDL3 box2d glm::glm Tracy::TracyClient spdlog::spdlog)

if(NOT MSVC)
    target_compile_options(Crane INTERFACE -Wall -Wextra -Wpedantic)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(Crane INTERFACE DEBUG)
        target_compile_options(Crane INTERFACE -O0 -g)
    else()
        target_compile_definitions(Crane INTERFACE NDEBUG)
        target_compile_options(Crane INTERFACE -O3)
    endif()
else()
    target_compile_options(Crane INTERFACE /W4 /MP)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(Crane INTERFACE DEBUG)
        target_compile_options(Crane INTERFACE /Od /Zi /Zf /MDd)
    else()
        target_compile_definitions(Crane INTERFACE NDEBUG)
    endif()
endif()

add_subdirectory("Samples")
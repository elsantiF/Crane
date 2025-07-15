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

file(GLOB_RECURSE SOURCE_FILES "Sources/*.cpp")

add_library(Crane ${SOURCE_FILES})
target_include_directories(Crane PUBLIC "Sources")
target_include_directories(Crane PUBLIC "External/entt/src")
target_link_libraries(Crane PUBLIC imgui SDL3::SDL3 box2d glm::glm Tracy::TracyClient)

if(NOT MSVC)
    target_compile_options(Crane PUBLIC -Wall -Wextra -Wpedantic)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(Crane PUBLIC DEBUG)
        target_compile_options(Crane PUBLIC -O0 -g)
    else()
        target_compile_definitions(Crane PUBLIC NDEBUG)
        target_compile_options(Crane PUBLIC -O3)
    endif()
else()
    target_compile_options(Crane PUBLIC /W4 /MP)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(Crane PUBLIC DEBUG)
        target_compile_options(Crane PUBLIC /Od /Zi /Zf /MDd)
    else()
        target_compile_definitions(Crane PUBLIC NDEBUG)
    endif()
endif()

add_subdirectory("Samples")
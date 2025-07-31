set(BASE_SOURCES
    "Profiler.cpp"
)

add_library(CraneBase STATIC ${BASE_SOURCES})
target_include_directories(CraneBase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneBase PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CraneBase PUBLIC Tracy::TracyClient spdlog::spdlog glm)
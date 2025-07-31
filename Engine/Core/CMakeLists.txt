set(CORE_SOURCES
    # Scene
    "Scene/World.cpp"
)

add_library(CraneCore STATIC ${CORE_SOURCES})
target_include_directories(CraneCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneCore PUBLIC "${CMAKE_SOURCE_DIR}/Crane")
target_link_libraries(CraneCore PUBLIC CraneBase SDL3::SDL3 EnTT::EnTT)

set(CORE_SOURCES
    # Application
    "Application/BaseApplication.cpp"
    # Scene
    "Scene/World.cpp"
)

add_library(CraneCore STATIC ${CORE_SOURCES})
target_include_directories(CraneCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneCore PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CraneCore PUBLIC CraneBase EnTT::EnTT)

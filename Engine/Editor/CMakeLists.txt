set(EDITOR_SOURCES 
    "EntityDisplay.cpp"
)

add_library(CraneEditor STATIC ${EDITOR_SOURCES})
target_include_directories(CraneEditor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneEditor PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CraneEditor PUBLIC CraneBase CranePhysics imgui EnTT::EnTT)

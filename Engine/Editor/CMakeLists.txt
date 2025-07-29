set(EDITOR_SOURCES 
    "EntityDisplay.cpp"
    "EntityDisplay.hpp"
)

add_library(CraneEditor STATIC ${EDITOR_SOURCES})
target_include_directories(CraneEditor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneEditor PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CraneEditor PUBLIC CraneBase CranePhysics)
target_link_libraries(CraneEditor PUBLIC imgui EnTT::EnTT)

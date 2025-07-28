set(EVENTS_SOURCES 
    "IEvent.hpp"
    "KeyDown.hpp"
    "KeyUp.hpp"
    "MouseClick.hpp"
)

add_library(CraneEvents INTERFACE)
target_sources(CraneEvents INTERFACE ${EVENTS_SOURCES})
target_include_directories(CraneEvents INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneEvents INTERFACE "${CMAKE_SOURCE_DIR}/Sources")

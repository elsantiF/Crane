set(CORE_SOURCES 
    # Application
    "Application/Application.cpp"
    "Application/Application.hpp"
    # Events
    "Events/IEvent.hpp"
    "Events/Events.hpp"
    # Systems
    "Systems/ISystem.hpp"
    "Systems/SystemManager.hpp"
    "Systems/SystemManager.cpp"
)

add_library(CraneCore STATIC ${CORE_SOURCES})
target_include_directories(CraneCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneCore PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CraneCore PUBLIC CraneBase CraneEditor CraneGraphics SDL3::SDL3 EnTT::EnTT)

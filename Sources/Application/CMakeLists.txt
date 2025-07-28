set(APPLICATION_SOURCES 
    "Application.cpp" 
    "Application.hpp"
)

add_library(CraneApplication STATIC ${APPLICATION_SOURCES})
target_include_directories(CraneApplication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(CraneApplication PUBLIC CraneMath CraneGraphics CraneWorld)
target_link_libraries(CraneApplication PUBLIC SDL3::SDL3 EnTT::EnTT)

set(APP_SOURCES
    "ClientApplication.cpp"
)

add_library(CraneApplication STATIC ${APP_SOURCES})
target_include_directories(CraneApplication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneApplication PUBLIC "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CraneApplication PUBLIC CraneCore CraneGraphics SDL3::SDL3 EnTT::EnTT)

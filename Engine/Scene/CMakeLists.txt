add_library(CraneScene INTERFACE)
target_include_directories(CraneScene INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneScene INTERFACE "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CraneScene INTERFACE CraneBase box2d EnTT::EnTT)
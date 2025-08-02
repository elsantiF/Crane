project(NBody)

add_executable(NBody "main.cpp" "GravitySystem.cpp")
target_link_libraries(NBody PUBLIC Crane)

add_custom_command(TARGET NBody POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
            "$<TARGET_FILE_DIR:NBody>/Resources")
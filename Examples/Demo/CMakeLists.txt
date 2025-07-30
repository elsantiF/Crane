project(Demo)

add_executable(Demo "main.cpp" "PlayerSystem.cpp")
target_link_libraries(Demo PUBLIC Crane stb)

add_custom_command(TARGET Demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
            "$<TARGET_FILE_DIR:Demo>/Resources")
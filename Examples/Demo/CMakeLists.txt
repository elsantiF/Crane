project(Demo)

add_executable(Demo "main.cpp" "PlayerSystem.cpp")
target_link_libraries(Demo PUBLIC Crane)
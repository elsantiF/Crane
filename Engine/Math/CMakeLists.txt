set(MATH_SOURCES 
    "Matrix.hpp"
    "Transform.hpp"
    "Vector.hpp"
)

add_library(CraneMath INTERFACE)
target_sources(CraneMath INTERFACE ${MATH_SOURCES})
target_include_directories(CraneMath INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CraneMath INTERFACE "${CMAKE_SOURCE_DIR}/Engine")
target_link_libraries(CraneMath INTERFACE glm)

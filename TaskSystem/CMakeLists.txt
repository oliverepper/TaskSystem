add_executable(cli main.cpp)
target_include_directories(cli PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/no"
    "${CMAKE_CURRENT_SOURCE_DIR}/simple"
    "${CMAKE_CURRENT_SOURCE_DIR}/scheduled"
    "${CMAKE_CURRENT_SOURCE_DIR}/stealing"
)
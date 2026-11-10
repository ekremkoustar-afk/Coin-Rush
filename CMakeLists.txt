cmake_minimum_required(VERSION 3.15)
project(coinRush CXX)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5)
FetchContent_MakeAvailable(raylib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_BINARY_DIR}/out>")
add_executable(coinRush coinRush.cpp)
target_link_libraries(coinRush raylib)

# Copy assets next to the exe (skipped if no resources folder)
if(EXISTS ${CMAKE_SOURCE_DIR}/resources)
  add_custom_command(TARGET coinRush POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:coinRush>/resources)
endif()

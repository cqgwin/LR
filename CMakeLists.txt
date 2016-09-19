# FTRL CMAKELISTS FILE
# 
cmake_minimum_required (VERSION 2.6)
project (lr)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_BINARY_DIR}"
)

add_subdirectory(src/io)
add_subdirectory(src/common)
add_subdirectory(src/ftrl)
add_subdirectory(src/sgd)

file(GLOB SOURCES 
    src/main.cc
    src/io/*.cc
    src/commom/*.cc
    src/ftrl/*.cc
    src/sgd/*.cc
)

add_executable(lr ${SOURCES})

target_link_libraries(lr io)
target_link_libraries(lr ftrl)
target_link_libraries(lr common)
target_link_libraries(lr sgd)

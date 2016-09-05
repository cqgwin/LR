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

add_subdirectory(io)
add_subdirectory(common)
add_subdirectory(ftrl)

file(GLOB SOURCES 
    main.cc
    io/*.cc
    commom/*.cc
    ftrl/*.cc
)

add_executable(lr ${SOURCES})

target_link_libraries(lr io)
target_link_libraries(lr ftrl)
target_link_libraries(lr common)

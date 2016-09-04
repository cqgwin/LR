# FTRL CMAKELISTS FILE
# 
cmake_minimum_required (VERSION 2.6)
project (lr)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

add_subdirectory(io)
add_subdirectory(common)
add_subdirectory(ftrl)

file(GLOB SOURCES 
    io/*.cc
    commom/*.cc
    ftrl/*.cc
)

add_executable(lr ${SOURCES})

target_link_libraries(lr io)
target_link_libraries(lr ftrl)
target_link_libraries(lr common)

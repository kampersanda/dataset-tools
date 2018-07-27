cmake_minimum_required(VERSION 2.8)

project(my_tools)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -pthread")

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif ()

message(STATUS "BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
message(STATUS "CXX_FLAGS are ${CMAKE_CXX_FLAGS}")
message(STATUS "CXX_FLAGS_DEBUG are ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CXX_FLAGS_RELEASE are ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CMAKE_INSTALL_PREFIX is ${CMAKE_INSTALL_PREFIX}")

add_executable(kamp_random_sample random_sample.cpp)
add_executable(kamp_down_sample down_sample.cpp)
add_executable(kamp_shuffle shuffle.cpp)
add_executable(kamp_sort sort.cpp)
add_executable(kamp_unique unique.cpp)
add_executable(kamp_statistics statistics.cpp)

install(TARGETS
        kamp_random_sample kamp_down_sample kamp_shuffle kamp_sort kamp_unique kamp_statistics
        RUNTIME DESTINATION bin)
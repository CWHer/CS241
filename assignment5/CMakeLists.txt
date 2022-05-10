project(assignment5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -O3 -std=c++11 -g")

add_executable(main Tree.h Tree.cpp main.cpp)
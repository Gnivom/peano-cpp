cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 17)

include_directories ("code/include")

add_executable (peano-cpp "code/main.cpp" "code/include/Peano.h")


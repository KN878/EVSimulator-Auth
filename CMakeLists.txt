cmake_minimum_required(VERSION 3.14)
project(EVSimulator)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")
find_package(CryptoPP REQUIRED)

add_executable(EVSimulator main.cpp)
target_link_libraries(EVSimulator cryptopp)

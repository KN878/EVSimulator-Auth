cmake_minimum_required(VERSION 3.14)
project(EVSimulator)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")
find_package(CryptoPP REQUIRED)

add_executable(EVSimulator main.cpp dmv/DMV.cpp car/Car.cpp car/Car.h chargingStation/ChargingStation.cpp chargingStation/ChargingStation.h chargingStation/CSPA.cpp chargingStation/CSPA.h car/obu/OBU.cpp car/obu/OBU.h car/trm/TRM.cpp car/trm/TRM.h dmv/DMV.h world/World.cpp world/World.h)
target_link_libraries(EVSimulator cryptopp)

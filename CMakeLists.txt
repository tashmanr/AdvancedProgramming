cmake_minimum_required(VERSION 3.16)
project(Advanced_Programming)

#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)
set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -std=c++11")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
add_executable(Advanced_Programming anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h timeseries.cpp timeseries.h MainTrain.cpp minCircle.cpp minCircle.h HybridAnomalyDetector.cpp HybridAnomalyDetector.h commands.h CLI.h CLI.cpp Server.h Server.cpp)
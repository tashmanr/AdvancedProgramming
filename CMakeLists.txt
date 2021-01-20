cmake_minimum_required(VERSION 3.16)
project(Advanced_Programming)

set(CMAKE_CXX_STANDARD 14)

add_executable(Advanced_Programming anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h timeseries.cpp timeseries.h MainTrain.cpp minCircle.cpp minCircle.h HybridAnomalyDetector.cpp HybridAnomalyDetector.h commands.h commands.cpp CLI.h CLI.cpp)
cmake_minimum_required(VERSION 3.6)
project(TLD)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(/usr/local/Cellar/opencv/2.4.13/include)
LINK_DIRECTORIES(/usr/local/Cellar/opencv/2.4.13/lib)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES
    source/detector/Detector.cpp
    source/detector/Detector.h
    source/learner/Learner.cpp
    source/learner/Learner.h
    source/tracker/MedianFlow/MedianFlow.cpp
    source/tracker/MedianFlow/MedianFlow.h
    source/tracker/MedianFlow/OpticalFlow.cpp
    source/tracker/MedianFlow/OpticalFlow.h
    source/video/VideoController.cpp
    source/video/VideoController.h
    source/video/ViewController.cpp
    source/video/ViewController.h
    source/utils/NNClassifier.cpp
    source/utils/NNClassifier.h
    source/utils/RandomFernsClassifier.cpp
    source/utils/RandomFernsClassifier.h
    source/utils/VarClassifier.cpp
    source/utils/VarClassifier.h
    source/main.cpp
    source/TLD.cpp
    source/TLD.h
    source/TLDSystemStruct.h)

add_executable(TLD ${SOURCE_FILES})
target_link_libraries(TLD ${OpenCV_LIBS})
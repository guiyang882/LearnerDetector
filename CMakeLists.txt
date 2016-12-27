cmake_minimum_required(VERSION 3.6)
project(TLD)

set(CMAKE_CXX_STANDARD 11)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "No Build Type selected, default to ${CMAKE_BUILD_TYPE}")
endif()

#set(RUN_TYPE "-DOUTPUT_STD")
#add_definitions(${RUN_TYPE})

INCLUDE_DIRECTORIES(/usr/local/Cellar/opencv/2.4.13.1/include)
LINK_DIRECTORIES(/usr/local/Cellar/opencv/2.4.13.1/lib)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES
    source/detector/Detector.cpp
    source/detector/Detector.h
    source/learner/Learner.cpp
    source/learner/Learner.h
    source/tracker/medianflow/Medianflow.cpp
    source/tracker/medianflow/Medianflow.h
    source/tracker/medianflow/OpticalFlow.cpp
    source/tracker/medianflow/OpticalFlow.h
    source/video/VideoController.cpp
    source/video/VideoController.h
    source/utils/NNClassifier.cpp
    source/utils/NNClassifier.h
    source/utils/RandomFernsClassifier.cpp
    source/utils/RandomFernsClassifier.h
    source/utils/VarClassifier.cpp
    source/utils/VarClassifier.h
    source/vibe/vibe-background-sequential.h
    source/vibe/vibe-background-sequential.c
    source/main.cpp
    source/TLD.cpp
    source/TLD.h
    source/TLDSystemStruct.h
    source/tracker/TrackerInstance.cpp
    source/tracker/TrackerInstance.h
    source/tracker/TrackerBase.cpp
    source/tracker/TrackerBase.h
    source/tracker/camshift/CamMeanShift.cpp
    source/tracker/camshift/CamMeanShift.h
    source/utils/utils.cpp
    source/utils/utils.h)

add_executable(TLD ${SOURCE_FILES})
target_link_libraries(TLD ${OpenCV_LIBS})
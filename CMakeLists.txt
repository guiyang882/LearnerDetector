cmake_minimum_required(VERSION 3.6)
project(TLD)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_COMPILER /usr/local/bin/clang-omp++)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "No Build Type selected, default to ${CMAKE_BUILD_TYPE}")
endif()

include_directories(/usr/local/include/libiomp)
link_directories(/usr/local/lib)

ADD_DEFINITIONS(-D_OPENMP)
#ADD_DEFINITIONS(-DCV_SHOW)
ADD_DEFINITIONS(-DOUTPUT_STD)

find_package(OpenCV 3 REQUIRED)

set(SOURCE_FILES
        source/detector/Detector.cpp
        source/detector/Detector.h
        source/learner/Learner.cpp
        source/learner/Learner.h
        source/utils/NNClassifier.cpp
        source/utils/NNClassifier.h
        source/utils/RandomFernsClassifier.cpp
        source/utils/RandomFernsClassifier.h
        source/utils/VarClassifier.cpp
        source/utils/VarClassifier.h
        source/tracker/TrackerBase.cpp
        source/tracker/TrackerBase.h
        source/utils/utils.cpp
        source/utils/utils.h
        source/utils/PatchGenerator.h
        source/utils/PatchGenerator.cpp
        source/utils/datareader.h
        source/utils/datareader.cpp
        source/TLD.cpp
        source/TLD.h
        source/TLDSystemStruct.h
        source/main.cpp)

add_executable(TLD ${SOURCE_FILES})
target_link_libraries(TLD ${OpenCV_LIBS} pthread iomp5)
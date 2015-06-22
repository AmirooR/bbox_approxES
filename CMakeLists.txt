cmake_minimum_required(VERSION 2.8)
#IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
IF(APPLE)
       # Mac OS X specific code
    SET(CMAKE_SYSTEM_NAME Darwin)

    # Add MacPorts
    INCLUDE_DIRECTORIES(/opt/local/include)
    LINK_DIRECTORIES(/opt/local/lib)
ENDIF(APPLE)
#ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

project(approximateES)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
find_package(OpenMP)
if (OPENMP_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package( OpenCV REQUIRED )
add_subdirectory(dense)


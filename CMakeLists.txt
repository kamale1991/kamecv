cmake_minimum_required(VERSION 3.16.0)
project(kamecv VERSION 0.1.0)


file(MAKE_DIRECTORY build)
file(MAKE_DIRECTORY build/bin)
file(MAKE_DIRECTORY build/lib)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_BUILD_FILES_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_BUILD_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_BINARY_DIR  ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_CACHEFILE_DIR ${CMAKE_BINARY_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# GoogleTest
#include(GoogleTest)
#include(CTest)
include_directories(/usr/src)
find_package(GTest REQUIRED)


# OpenMP
find_package(OpenMP REQUIRED)
#target_link_libraries(mptest PUBLIC OpenMP::OpenMP_CXX)

# MPI
find_package(MPI REQUIRED)

# MKL
include_directories(/opt/intel/mkl/include)
find_library(MKL_LIB /opt/intel/mkl/lib/intel64)

# Eigen
find_package(Eigen3 REQUIRED)

# OpenCV
#include_directories(/usr/include/opencv4)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})    

# Boost
find_package(Boost COMPONENTS program_options REQUIRED)

# Python
include_directories(/usr/include/python3.8)

# add_subdirectoryでそれぞれのプロジェクトディレクトリを登録する
file(GLOB dir_list LIST_DIRECTORIES true RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./*)
set(dir_list math PointShape Game)
foreach(dir ${dir_list})       
    message("add subdirectory " ${dir}) 
    add_subdirectory(${dir})
endforeach()

file(GLOB src_service "service/src/*.cpp")

## PROJECT_NAMEと同じ名前の実行体にmain.cppをリンクする
add_executable(${PROJECT_NAME} 
    main.cpp
    service/src/
    )
target_link_libraries(${PROJECT_NAME} 
    MPI::MPI_CXX
    ${OpenCV_LIBRARIES}
)

add_executable(figure_experiment 
    main_figure_experiment.cpp
    ${src_service}
    )
target_link_libraries(figure_experiment
    MPI::MPI_CXX
    ${OpenCV_LIBRARIES}
		mymath
		point_shape
		Eigen3::Eigen
		Boost::program_options
)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
enable_testing()
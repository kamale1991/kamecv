cmake_minimum_required(VERSION 3.16.0)
project(cellular_automaton VERSION 0.1.0)
include(GoogleTest)
include(CTest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)

# GoogleTest
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

# OpenCV
#include_directories(/usr/include/opencv4)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})    

# Python
include_directories(/usr/include/python3.8)

# add_subdirectoryでそれぞれのプロジェクトディレクトリを登録する
file(GLOB dir_list LIST_DIRECTORIES true RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./*)
set(dir_list cellular_automaton;math;PointShape;Game)
foreach(dir ${dir_list})       
    message("add subdirectory "${dir}) 
    add_subdirectory(${dir})

    file(GLOB srcs_${dir} "${dir}/src/*.cpp")
    list(APPEND srcs srcs_${dir})
endforeach()

message(${srcs})
## PROJECT_NAMEと同じ名前の実行体にmain.cppをリンクする
add_executable(${PROJECT_NAME} 
    main.cpp
    ${${srcs}}
    )
target_link_libraries(${PROJECT_NAME} 
    MPI::MPI_CXX
    ${OpenCV_LIBRARIES}
)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
enable_testing()
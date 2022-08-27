cmake_minimum_required(VERSION 3.16.0)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("Here is "${DIR_NAME})
set(DIR_NAME cellular_automaton)

file(GLOB_RECURSE includes include/*.h)
file(GLOB_RECURSE srcs src/*.cpp)
set(DIR_MAIN ${DIR_NAME}_main)

add_executable(${DIR_MAIN} ${srcs} ${DIR_NAME}_main.cpp)
target_link_libraries(${DIR_MAIN}
    ${OpenCV_LIBRARIES}    
)

add_library(ca ${srcs})
set_target_properties(ca PROPERTIES VERSION ${PROJECT_VERSION})
target_include_directories(ca PUBLIC include)

# ./testのビルドへ進む
add_subdirectory(test)
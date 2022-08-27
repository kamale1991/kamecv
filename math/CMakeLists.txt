cmake_minimum_required(VERSION 3.16.0)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("Here is "${DIR_NAME})
set(DIR_NAME template)

file(GLOB_RECURSE srcs include/*.h src/*.cpp)
add_executable(${DIR_NAME} ${srcs} ${DIR_NAME}_main.cpp)

# ./testのビルドへ進む
add_subdirectory(test)
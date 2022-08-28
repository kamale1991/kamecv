cmake_minimum_required(VERSION 3.16.0)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("Here is "${DIR_NAME})
set(DIR_NAME math)

file(GLOB_RECURSE srcs include/*.h src/*.cpp)
add_executable(${DIR_NAME} ${srcs} ${DIR_NAME}_main.cpp)

# mathをライブラリにする
add_library(mymath ${srcs})
set_target_properties(mymath PROPERTIES VERSION ${PROJECT_VERSION})
target_include_directories(mymath PUBLIC include)

# ./testのビルドへ進む
add_subdirectory(test)
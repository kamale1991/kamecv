cmake_minimum_required(VERSION 3.16.0)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("Here is "${DIR_NAME})
set(DIR_NAME Game)

file(GLOB_RECURSE srcs include/*.cpp include/*.h)

add_executable(game_comm ${srcs} game_comm.cpp)
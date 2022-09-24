cmake_minimum_required(VERSION 3.16.0)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("Here is "${DIR_NAME})
#set(DIR_NAME PointShape)

file(GLOB_RECURSE srcs include/*.h src/*.cpp)
#add_executable(animate_heapsort ${srcs} animate_heapsort.cpp)
#target_link_libraries(animate_heapsort
#	glut	
#	GLU
#	GL	
#)


# mathをライブラリにする
add_library(point_shape ${srcs})
set_target_properties(point_shape PROPERTIES VERSION ${PROJECT_VERSION})
target_include_directories(point_shape PUBLIC include)

add_executable(morph_main ${srcs} morph_main.cpp)
target_link_libraries(morph_main
 point_shape
)


# ./testのビルドへ進む
add_subdirectory(test)
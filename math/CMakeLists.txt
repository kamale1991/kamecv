cmake_minimum_required(VERSION 3.16.0)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("Here is "${DIR_NAME})
set(DIR_NAME math)

#file(GLOB_RECURSE srcs include/*.h src/*.cpp)
file(GLOB_RECURSE srcs src/*.cpp)
add_executable(animate_heapsort ${srcs} animate_heapsort.cpp)
target_link_libraries(animate_heapsort
	glut	
	GLU
	GL	
)

add_executable(bayse_opt ${srcs} bayse_opt.cpp)
target_link_libraries(bayse_opt
	glut	
	GLU
	GL	
)

# mathをライブラリにする
add_library(mymath ${srcs})
set_target_properties(mymath PROPERTIES VERSION ${PROJECT_VERSION})
target_include_directories(mymath PUBLIC include)

# ./testのビルドへ進む
add_subdirectory(test)
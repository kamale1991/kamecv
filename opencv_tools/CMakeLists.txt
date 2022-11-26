cmake_minimum_required(VERSION 3.16.0)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("Here is "${DIR_NAME})
set(DIR_NAME opencv_test)

file(GLOB_RECURSE includes include/*.h)
file(GLOB_RECURSE srcs src/*.cpp)
set(DIR_MAIN ${DIR_NAME}_main)

add_executable(celluar_automaton ${srcs} cellulat_automaton.cpp)
target_link_libraries(cellular_automaton
    ${OpenCV_LIBRARIES}    
)

add_executable(dft_tool ${srcs} dft_tool.cpp)
target_link_libraries(dft_tool
		${OpenCV_LIBRARIES}
)

add_library(ca ${srcs})
set_target_properties(ca PROPERTIES VERSION ${PROJECT_VERSION})
target_include_directories(ca PUBLIC include)

# ./testのビルドへ進む
add_subdirectory(test)
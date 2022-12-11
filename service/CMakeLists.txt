cmake_minimum_required(VERSION 3.16.0)
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("Here is " ${DIR_NAME})

# ここは何もビルドしない。
# srcの読み込みは上位に任せる
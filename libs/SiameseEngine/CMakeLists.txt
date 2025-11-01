cmake_minimum_required(VERSION 4.2)
project(SiameseEngine LANGUAGES CXX)

#create the library
add_library(SiameseEngine)

#get the source files in the folder
file(GLOB_RECURSE INCLUDE_FILES CONFIGURE_DEPENDS "include/*.h")
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

#add the files to the project
target_sources(SiameseEngine
    PRIVATE ${SRC_FILES}
    PUBLIC ${INCLUDE_FILES}
)

#include dir
target_include_directories(SiameseEngine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_source_group_by_folder("${INCLUDE_FILES}" "${CMAKE_CURRENT_SOURCE_DIR}/include")
add_source_group_by_folder("${SRC_FILES}" "${CMAKE_CURRENT_SOURCE_DIR}/src")

#cpp std
target_compile_features(SiameseEngine PUBLIC cxx_std_20)

#pch
target_precompile_headers(SiameseEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/sepch.h")
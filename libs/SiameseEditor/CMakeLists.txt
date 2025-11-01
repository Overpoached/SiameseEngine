cmake_minimum_required(VERSION 4.2)
project(SiameseEditor LANGUAGES CXX)

#create the library
add_executable(SiameseEditor)

#get the source files in the folder
file(GLOB_RECURSE INCLUDE_FILES CONFIGURE_DEPENDS "include/*.h")
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

#add the files to the project
target_sources(SiameseEditor
    PRIVATE ${SRC_FILES}
    PUBLIC ${INCLUDE_FILES}
)

#include dir
target_include_directories(SiameseEditor
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_source_group_by_folder("${INCLUDE_FILES}" "${CMAKE_CURRENT_SOURCE_DIR}/include")
add_source_group_by_folder("${SRC_FILES}" "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(SiameseEditor
    PUBLIC SiameseEngine
)

#cpp std
target_compile_features(SiameseEditor PUBLIC cxx_std_20)

#pch
target_precompile_headers(SiameseEditor REUSE_FROM SiameseEngine)
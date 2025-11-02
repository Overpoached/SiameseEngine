cmake_minimum_required(VERSION 4.2)
project(SiameseEngine)

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

add_subdirectory(libs/SiameseEngine)
add_subdirectory(libs/SiameseEditor)

add_subdirectory(external/spdlog)
set_target_properties(spdlog PROPERTIES FOLDER "external")
add_subdirectory(external/inifile_cpp)
set_target_properties(inicpp PROPERTIES FOLDER "external")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SiameseEditor)
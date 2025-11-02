cmake_minimum_required(VERSION 4.2)
project(SiameseEngine)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "" FORCE)
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

add_subdirectory(libs/SiameseEngine)
add_subdirectory(libs/SiameseEditor)
add_subdirectory(libs/SiameseRenderer)

target_compile_definitions(SiameseEngine PUBLIC
    $<$<CONFIG:Debug>:SE_DEBUG>
    $<$<CONFIG:Release>:SE_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:SE_RELWITHDEBINFO>
)
target_compile_definitions(SiameseEditor PUBLIC
    $<$<CONFIG:Debug>:SE_DEBUG>
    $<$<CONFIG:Release>:SE_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:SE_RELWITHDEBINFO>
)
target_compile_definitions(SiameseRenderer PUBLIC
    $<$<CONFIG:Debug>:SE_DEBUG>
    $<$<CONFIG:Release>:SE_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:SE_RELWITHDEBINFO>
)

add_subdirectory(external/spdlog)
set_target_properties(spdlog PROPERTIES FOLDER "external")
add_subdirectory(external/inifile_cpp)
set_target_properties(inicpp PROPERTIES FOLDER "external")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SiameseEditor)
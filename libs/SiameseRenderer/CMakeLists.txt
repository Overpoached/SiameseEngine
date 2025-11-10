cmake_minimum_required(VERSION 4.2)
project(SiameseRenderer LANGUAGES CXX)

#create the library
add_executable(SiameseRenderer)
set_target_properties(SiameseRenderer PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

#get the source files in the folder
file(GLOB_RECURSE INCLUDE_FILES CONFIGURE_DEPENDS "include/*.h")
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
# Get all window wrapper files
file(GLOB_RECURSE SHARED_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/libs/shared/*.h" "${CMAKE_SOURCE_DIR}/libs/shared/*.cpp")

#add the files to the project
target_sources(SiameseRenderer PRIVATE
    ${SRC_FILES}
    ${INCLUDE_FILES}
    ${SHARED_FILES}
)

#include dir
target_include_directories(SiameseRenderer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libs/shared
)

target_link_libraries(SiameseRenderer PRIVATE
    SiameseEngine
    glfw
    tinygltf
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "Header Files" FILES ${INCLUDE_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SRC_FILES})

get_target_property(SIAMESEENGINE_HEADERS SiameseEngine INTERFACE_SOURCES)
source_group(TREE ${CMAKE_SOURCE_DIR}/libs/SiameseEngine PREFIX "SiameseEngine" FILES ${SIAMESEENGINE_HEADERS})

source_group(TREE ${CMAKE_SOURCE_DIR}/libs/shared PREFIX "Shared" FILES ${SHARED_FILES})

#cpp std
target_compile_features(SiameseRenderer PUBLIC cxx_std_20)
if(MSVC)
    add_compile_options(/MP)
endif()

#pch
target_precompile_headers(SiameseRenderer REUSE_FROM SiameseEngine)
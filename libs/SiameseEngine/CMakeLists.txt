cmake_minimum_required(VERSION 4.2)
project(SiameseEngine LANGUAGES CXX)

#get the source files in the folder
file(GLOB_RECURSE INCLUDE_FILES CONFIGURE_DEPENDS "include/*.h")
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

#create the library
add_library(SiameseEngine)

#add the files to the project
target_sources(SiameseEngine
    PUBLIC ${INCLUDE_FILES}
    PRIVATE ${SRC_FILES}
)

#include dir
target_include_directories(SiameseEngine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#link with other libraries
target_link_libraries(SiameseEngine PUBLIC
    spdlog::spdlog
    inicpp
    MathLib
    EnTT
    nvrhi
    nvrhi_d3d12
    DirectX-Headers
    dxgi
)
target_link_libraries(SiameseEngine PRIVATE
    DbgHelp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "Header Files" FILES ${INCLUDE_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SRC_FILES})

#cpp std
target_compile_features(SiameseEngine PUBLIC cxx_std_20)
if(MSVC)
    add_compile_options(/MP)
endif()

#pch
target_precompile_headers(SiameseEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/sepch.h")
cmake_minimum_required(VERSION 4.2)
project(SiameseEditor LANGUAGES CXX)

#create the library
add_executable(SiameseEditor)
set_target_properties(SiameseEditor PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

#get the source files in the folder
file(GLOB_RECURSE INCLUDE_FILES CONFIGURE_DEPENDS "include/*.h")
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

#add the files to the project
target_sources(SiameseEditor PRIVATE ${SRC_FILES} ${INCLUDE_FILES})

#include dir
target_include_directories(SiameseEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(SiameseEditor PUBLIC SiameseEngine)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "Header Files" FILES ${INCLUDE_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SRC_FILES})

#add the dependency in a diff folder
get_target_property(SIAMESEENGINE_HEADERS SiameseEngine INTERFACE_SOURCES)
source_group("Header Files/SiameseEngine" FILES ${SIAMESEENGINE_HEADERS})

#cpp std
target_compile_features(SiameseEditor PUBLIC cxx_std_20)
if(MSVC)
    add_compile_options(/MP)
endif()

#pch
target_precompile_headers(SiameseEditor REUSE_FROM SiameseEngine)
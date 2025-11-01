cmake_minimum_required(VERSION 4.2)
project(SiameseEngine)

#function to assign source groups based on folder structure
function(add_source_group_by_folder files)
    foreach(f ${files})
        file(RELATIVE_PATH rel "${CMAKE_CURRENT_SOURCE_DIR}" "${f}")
        get_filename_component(dir "${rel}" PATH)
        string(REPLACE "/" "\\" dir_filter "${dir}")
        source_group("${dir_filter}" FILES "${f}")
    endforeach()
endfunction()

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

add_subdirectory(libs/SiameseEngine)
add_subdirectory(libs/SiameseEditor)
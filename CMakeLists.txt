cmake_minimum_required(VERSION 4.2)
project(SiameseEngine)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "" FORCE)
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

add_subdirectory(external/inifile_cpp)

add_subdirectory(external/spdlog)
set_target_properties(spdlog PROPERTIES FOLDER "external/spdlog")

set(GLFW_INSTALL OFF CACHE INTERNAL "" FORCE)
add_subdirectory(external/glfw)
set_target_properties(glfw PROPERTIES FOLDER "external/glfw")
set_target_properties(update_mappings PROPERTIES FOLDER "external/glfw")

add_subdirectory(external/MathLib)
set_target_properties(MathLib PROPERTIES FOLDER "external/MathLib")

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE INTERNAL "" FORCE)
add_subdirectory(external/tinygltf)
set_target_properties(tinygltf PROPERTIES FOLDER "external/tinygltf")

set(NVRHI_WITH_DX12 ON CACHE INTERNAL "" FORCE)
set(NVRHI_WITH_DX11 OFF CACHE INTERNAL "" FORCE)
set(NVRHI_WITH_VULKAN OFF CACHE INTERNAL "" FORCE)
set(NVRHI_WITH_AFTERMATH OFF CACHE INTERNAL "" FORCE)
add_subdirectory(external/NVRHI)
set_target_properties(nvrhi PROPERTIES FOLDER "external/NVRHI")
set_target_properties(nvrhi_d3d12 PROPERTIES FOLDER "external/NVRHI")
set_target_properties(DirectX-Guids PROPERTIES FOLDER "external/NVRHI")
set_target_properties(DirectX-Headers PROPERTIES FOLDER "external/NVRHI")

add_subdirectory(external/entt)
set_target_properties(EnTT PROPERTIES FOLDER "external/entt")

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

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SiameseRenderer)
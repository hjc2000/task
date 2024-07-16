cmake_minimum_required(VERSION 3.20)
include(${cpp_lib_build_scripts_path}/cmake-module/setup.cmake)
include(target_import_freertos)
include(target_import_base)
include(target_import_libstm32h743_hal)

set(ProjectName task)
project(${ProjectName})
enable_language(ASM)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

add_library(${ProjectName} STATIC)
target_import_src(${ProjectName})
target_import_freertos(${ProjectName} PUBLIC)
target_import_base(${ProjectName} PUBLIC)
target_import_libstm32h743_hal(${ProjectName} PUBLIC)

set(CMAKE_INSTALL_PREFIX ${cpp_lib_build_scripts_path}/${platform}/.libs/${ProjectName}/)
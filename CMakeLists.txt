cmake_minimum_required(VERSION 3.22)

project(slow-is-fast)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS NO)

if (MSVC)
    add_compile_options(/std:c++latest)
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE) 


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include(config)
include(options)
include(compilerflags)


add_subdirectory(3rdParty)
add_subdirectory(src)

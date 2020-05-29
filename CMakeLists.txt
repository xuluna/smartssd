cmake_minimum_required(VERSION 3.0)

set(DM_REGION_LOG_GRAIN_SIZE 25 CACHE STRING "log base 2 of the device_manager allocation granularity")

if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR AND FORCE_OTT_BUILD)
    message(FATAL_ERROR "Cannot use in-source build ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}. You should delete any CMakeCache.txt and CMakeFiles and then try out-of-tree build")
endif(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR AND FORCE_OTT_BUILD)

add_compile_options(-g)
add_compile_options(-Wall)

add_subdirectory(kernel)
add_subdirectory(host)
add_subdirectory(client)

cmake_minimum_required(VERSION 3.5)

set(DM_REGION_LOG_GRAIN_SIZE 25 CACHE STRING "log base 2 of the device_manager allocation granularity")

if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR AND FORCE_OTT_BUILD)
    message(FATAL_ERROR "Cannot use in-source build ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}. You should delete any CMakeCache.txt and CMakeFiles and then try out-of-tree build")
endif(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR AND FORCE_OTT_BUILD)

add_compile_options(-g)
add_compile_options(-Wall)

project(compression)

find_package(OpenCL REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(XILINX_XRT /opt/xilinx/xrt)
set(XILINX_VITIS ${HOME}/xilinx/Vitis/2019.2)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${XILINX_XRT}/include)
include_directories(${XILINX_VITIS}/include)
include_directories(${OpenCL_INCLUDE_DIRS})

link_directories(${XILINX_XRT}/lib)

add_definitions(-DCONFIG_DEBUG)

file(GLOB SOURCES src/*.c*)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCL_LIBRARIES} pthread rt stdc++)

target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:-O0>")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
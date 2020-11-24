cmake_minimum_required(VERSION 3.0)

project(compression-host)

find_package(OpenCL REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(XILINX_XRT /opt/xilinx/xrt)
set(XILINX_VITIS ${HOME}/xilinx/Vitis/2019.2)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${OpenCL_INCLUDE_DIRS})
include_directories(${XILINX_XRT}/include)

link_directories(${XILINX_XRT}/lib)

file(GLOB SOURCES src/*.c*)

add_library(${PROJECT_NAME} SHARED src/lz4_p2p_comp.cpp src/lz4_p2p_dec.cpp src/xcl2.cpp src/xxhash.c)
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:-O0>")
target_link_libraries(${PROJECT_NAME} OpenCL ${OpenCL_LIBRARIES} pthread rt)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
install(DIRECTORY "include/" DESTINATION include FILES_MATCHING PATTERN "*.h*")
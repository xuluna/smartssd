cmake_minimum_required(VERSION 3.0)

project(compression-client)

find_package(OpenCL REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(XILINX_XRT /opt/xilinx/xrt)
set(XILINX_VITIS ${HOME}/xilinx/Vitis/2019.2)

include_directories(${CMAKE_INSTALL_PREFIX}/include)
include_directories(${OpenCL_INCLUDE_DIRS})
include_directories(${XILINX_XRT}/include)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)

file(GLOB SOURCES src/*.c*)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} compression-host pthread dl boost_program_options)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
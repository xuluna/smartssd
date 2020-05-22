cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_COMPILER "$ENV{HOME}/xilinx/Vitis/2019.2/bin/v++")
set(CMAKE_CXX_COMPILER "$ENV{HOME}/xilinx/Vitis/2019.2/bin/v++")

project(compression-kernel)

set(XILINX_XRT /opt/xilinx/xrt)
set(XILINX_VITIS $ENV{HOME}/xilinx/Vitis/2019.2)
set(PLATFORM ${XILINX_VITIS}/platforms/xilinx_samsung_u2x4_201920_2/xilinx_samsung_u2x4_201920_2.xpfm)
set(KERNELNAME "xilLz4Compress")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#include_directories(${XILINX_XRT}/include)
#include_directories(${XILINX_VITIS}/include)

add_library(xf_compress.xo OBJECT src/lz4_compress_mm.cpp)
target_compile_options(xf_compress.xo PUBLIC -t hw --platform ${PLATFORM} -k ${KERNELNAME})
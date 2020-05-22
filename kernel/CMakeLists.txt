cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_COMPILER "$ENV{HOME}/xilinx/Vitis/2019.2/bin/v++")
set(CMAKE_CXX_COMPILER "$ENV{HOME}/xilinx/Vitis/2019.2/bin/v++")

project(compression-kernel)

set(XILINX_XRT /opt/xilinx/xrt)
set(XILINX_VITIS $ENV{HOME}/xilinx/Vitis/2019.2)
set(TARGET sw_emu)
set(PLATFORM ${XILINX_VITIS}/platforms/xilinx_samsung_u2x4_201920_2/xilinx_samsung_u2x4_201920_2.xpfm)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(xf_compress STATIC src/lz4_compress_mm.cpp)
target_compile_options(xf_compress PUBLIC -t ${TARGET} --platform ${PLATFORM} -k xilLz4Compress)
set_target_properties(xf_compress PROPERTIES SUFFIX ".xo")

add_library(xf_packer STATIC src/lz4_packer_mm.cpp)
target_compile_options(xf_packer PUBLIC -t ${TARGET} --platform ${PLATFORM} -k xilLz4Packer)
set_target_properties(xf_packer PROPERTIES SUFFIX ".xo")

add_custom_target(compress ALL 
COMMAND ${CMAKE_CXX_COMPILER} -t ${TARGET} --platform ${PLATFORM} --config ${CMAKE_CURRENT_SOURCE_DIR}/compress.ini -o compress.xclbin -I${CMAKE_CURRENT_SOURCE_DIR}/include/ -l libxf_compress.xo libxf_packer.xo
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
DEPENDS xf_compress xf_packer
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/compress.xclbin DESTINATION lib)
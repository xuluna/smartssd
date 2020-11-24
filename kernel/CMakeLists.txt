cmake_minimum_required(VERSION 3.0)

project(compression-kernel)

set(CMAKE_C_COMPILER "$ENV{HOME}/xilinx/Vitis/2019.2/bin/v++")
set(CMAKE_CXX_COMPILER "$ENV{HOME}/xilinx/Vitis/2019.2/bin/v++")


set(XILINX_XRT /opt/xilinx/xrt)
set(XILINX_VITIS $ENV{HOME}/xilinx/Vitis/2019.2)
set(TARGET hw)
set(PLATFORM ${XILINX_VITIS}/platforms/xilinx_samsung_u2x4_201920_2/xilinx_samsung_u2x4_201920_2.xpfm)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_custom_target(xf_compress ALL
COMMAND ${CMAKE_CXX_COMPILER} -t ${TARGET} --platform ${PLATFORM} -k xilLz4Compress -I${CMAKE_CURRENT_SOURCE_DIR}/include/ -o xf_compress.xo -c ${CMAKE_CURRENT_SOURCE_DIR}/src/lz4_compress_mm.cpp
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(xf_packer ALL
COMMAND ${CMAKE_CXX_COMPILER} -t ${TARGET} --platform ${PLATFORM} -k xilLz4Packer -I${CMAKE_CURRENT_SOURCE_DIR}/include/ -o xf_packer.xo -c ${CMAKE_CURRENT_SOURCE_DIR}/src/lz4_packer_mm.cpp
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(xf_uncompress ALL
COMMAND ${CMAKE_CXX_COMPILER} -t ${TARGET} --platform ${PLATFORM} -k xilLz4P2PDecompress -I${CMAKE_CURRENT_SOURCE_DIR}/include/ -o xf_uncompress.xo -c ${CMAKE_CURRENT_SOURCE_DIR}/src/lz4_p2p_decompress_kernel.cpp
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(xf_unpacker ALL
COMMAND ${CMAKE_CXX_COMPILER} -t ${TARGET} --platform ${PLATFORM} -k xilLz4Unpacker -I${CMAKE_CURRENT_SOURCE_DIR}/include/ -o xf_unpacker.xo -c ${CMAKE_CURRENT_SOURCE_DIR}/src/lz4_unpacker_kernel.cpp
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(compress ALL 
COMMAND ${CMAKE_CXX_COMPILER} -t ${TARGET} --platform ${PLATFORM} --config ${CMAKE_CURRENT_SOURCE_DIR}/compression.ini -o compression.xclbin -I${CMAKE_CURRENT_SOURCE_DIR}/include/ -l xf_compress.xo xf_packer.xo xf_uncompress.xo xf_unpacker.xo
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
DEPENDS xf_compress xf_packer xf_uncompress xf_unpacker
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/compress.xclbin DESTINATION bin)
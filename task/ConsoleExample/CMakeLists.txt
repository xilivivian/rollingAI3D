#
# Linux ConsoleExample
#

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project("ConsoleExample")

set (CMAKE_CXX_STANDARD 14)

set(SOURCES
	FrameBuffer.cpp
	ConsoleExample.cpp
	$ENV{FSSDK_PATH}/Lib/bitmap/bitmap_image.hpp
)

if (UNIX)
    set(FSSDK_HEADERS $ENV{FSSDK_PATH}/API $ENV{FSSDK_PATH}/Lib/bitmap)
    set(FSSDK_LIBS $ENV{FSSDK_PATH}/API/libvevobase.so $ENV{FSSDK_PATH}/API/libvevo.so)
    set(IPP_LIB_PATH $ENV{IPP_PATH}/lib/intel64)
    set(IPP_LIBS ${IPP_LIB_PATH}/libippcc.so ${IPP_LIB_PATH}/libippch.so ${IPP_LIB_PATH}/libippcore.so ${IPP_LIB_PATH}/libippcv.so ${IPP_LIB_PATH}/libippdc.so ${IPP_LIB_PATH}/libippi.so ${IPP_LIB_PATH}/libipps.so ${IPP_LIB_PATH}/libippvm.so)
endif() # UNIX


add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${FSSDK_HEADERS})

target_link_libraries(${PROJECT_NAME} pthread dl ${FSSDK_LIBS} ${IPP_LIBS})

# Copy log configuration file
file(COPY vevolog.conf DESTINATION .)

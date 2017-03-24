#***************************************************************************
# Copyright:  Kenneth Lausdahl,
# E-Mail:     lausdahl@eng.au.dk
# Year:       2017
#***************************************************************************
cmake_minimum_required(VERSION 2.8)


# Set a consistent MACOSX_RPATH default across all CMake versions. 
# When CMake 2.8.12 is required, change this default to 1.
# When CMake 3.0.0 is required, remove this block (see CMP0042).
if(NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH 0)
endif()

project("minizip")
set(CMAKE_VERBOSE_MAKEFILE on)

# set cmake debug postfix to d
set(CMAKE_DEBUG_POSTFIX "d")

# Ensure correct version of zlib is referenced
set(ZLIB_ROOT ${DEF_ZLIB_ROOT} CACHE PATH "Parent directory of zlib installation")
find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
endif()


include_directories(../../zlib/contrib/minizip)
if (WIN32)
	#windows, becuase windows just cannot figure out to do it correct. 
	# must be a bug in msys mingw gcc 6.3	
	# it doesnt read the rsp files.
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I../../zlib/contrib/minizip")
endif() 


set(MINIZIP_SRC "../../zlib/contrib/minizip/ioapi.c"
"miniunz.c"
 # "minizip.c"
  "../../zlib/contrib/minizip/mztools.c"
  "../../zlib/contrib/minizip/unzip.c"
  "../../zlib/contrib/minizip/zip.c")

#set(MINIZIP_PUBLIC_HEADERS "crypt.h"
#  "ioapi.h"
#  "ioapi_buf.h"
#  "ioapi_mem.h"
#  "unzip.h"
#  "zip.h")

if(WIN32)
  list(APPEND MINIZIP_SRC "../../zlib/contrib/minizip/iowin32.c")
  list(APPEND MINIZIP_PUBLIC_HEADERS "../../zlib/contrib/minizip/iowin32.h")
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif()

if(UNIX)
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -O3")
endif()

# create minizip library
add_library(minizip ${MINIZIP_SRC} )#${MINIZIP_PUBLIC_HEADERS}
if (WIN32)
	target_link_libraries(minizip libz.a)
else()
	target_link_libraries(minizip ZLIB::ZLIB)
endif()
target_include_directories(minizip
  PUBLIC
   "${CMAKE_CURRENT_SOURCE_DIR}"  
 )





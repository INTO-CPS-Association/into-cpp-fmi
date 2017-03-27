#***************************************************************************
# Copyright:  Kenneth Lausdahl,
# E-Mail:     lausdahl@eng.au.dk
# Year:       2017
#***************************************************************************
cmake_minimum_required(VERSION 2.8)

project("zlib")


FILE(GLOB ZLIB_SRC *.c)

add_library(${PROJECT_NAME} ${ZLIB_SRC})


#target_link_libraries (${PROJECT_NAME} minizip)
target_include_directories(${PROJECT_NAME} PUBLIC include/fmi/include src/)
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)


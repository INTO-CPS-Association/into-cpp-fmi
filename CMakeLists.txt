cmake_minimum_required (VERSION 2.8.5)
project (fmu C CXX)

include(CheckCXXCompilerFlag)

set(CMAKE_VERBOSE_MAKEFILE on)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFMI_COSIMULATION -O0 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFMI_COSIMULATION -O0 -g")

add_subdirectory (thrird_party/minizip)

include_directories(include/fmi/include thrird_party/minizip)



file(GLOB C_FILES src/*.c) 
file(GLOB CPP_FILES src/*.cpp)

add_executable(${PROJECT_NAME}_main ${C_FILES} thrird_party/minizip/minishared.c ${CPP_FILES})
target_link_libraries (${PROJECT_NAME}_main minizip)



foreach(LETTER ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})
	
if(EXISTS "${LETTER}/libstdc++.a")
#   message (STATUS "${LETTER}")
	set(STDCPP_LIBRARY "${LETTER}/libstdc++.a")
else()
#  message (STATUS "${LETTER} ---- not these")
endif()
endforeach()

message(STATUS "Static linking with libstdc++.a in ${STDCPP_LIBRARY}")
target_link_libraries(${PROJECT_NAME}_main ${STDCPP_LIBRARY})


# I need this: -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic

if (WIN32)
	#windows	
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
endif() 

# Configure exposed dependency information to other projects

file(GLOB CPP_FILES_LIB src/*.cpp)
list(REMOVE_ITEM CPP_FILES_LIB "src/main.cpp")

add_library(${PROJECT_NAME} ${C_FILES} thrird_party/minizip/minishared.c ${CPP_FILES_LIB})
target_link_libraries (${PROJECT_NAME} minizip)
target_include_directories(${PROJECT_NAME} PUBLIC include/fmi/include src/)
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)


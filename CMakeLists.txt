cmake_minimum_required (VERSION 2.8.5)
project (intocppfmi C CXX)

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

add_definitions(-DFMI_COSIMULATION)

add_subdirectory (thrird_party/projects/minizip)

include_directories(include/fmi/include)
if (WIN32)
	#windows, becuase windows just cannot figure out to do it correct. 
	# must be a bug in msys mingw gcc 6.3	
	# it doesnt read the rsp files.
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Iinclude/fmi/include")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Iinclude/fmi/include -I${minizip_SOURCE_DIR}")
endif() 

if (WIN32)
	#windows	
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
endif() 

# Configure exposed dependency information to other projects

file(GLOB C_FILES src/*.c) 
file(GLOB CPP_FILES_LIB src/*.cpp)

add_library(${PROJECT_NAME} ${C_FILES} ${CPP_FILES_LIB})
target_link_libraries (${PROJECT_NAME} minizip)
target_include_directories(${PROJECT_NAME} PUBLIC include/fmi/include src/)
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)


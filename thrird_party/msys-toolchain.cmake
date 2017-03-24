# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)

SET(WIN32 true)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER /mingw64/bin/gcc)
SET(CMAKE_CXX_COMPILER /mingw64/bin/g++)
#SET(CMAKE_RC_COMPILER i486-mingw32-windres)

# here is the target environment located
#SET(CMAKE_FIND_ROOT_PATH /usr/i486-mingw32)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

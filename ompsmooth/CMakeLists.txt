CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#set the generator to build for eclipse projects
#set(CMAKE_ECLIPSE_VERSION 3.7.2)
#set(CMAKE_GENERATOR "Eclipse CDT4 - Unix Makefiles")

PROJECT(ompsmooth C)

#ADD_DEFINITIONS(-g -O2 -fsigned-char -freg-struct-return -Wall -W -Wshadow -Wstrict-prototypes -Wpointer-arith -Wcast-qual -Winline -Werror)
ADD_DEFINITIONS(-fopenmp)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

ADD_LIBRARY(${PROJECT_NAME} ompsmooth.c)
ADD_EXECUTABLE(smooth smooth.c)
TARGET_LINK_LIBRARIES(smooth ${LIBRARY_OUTPUT_PATH}/lib${PROJECT_NAME}.a gomp)

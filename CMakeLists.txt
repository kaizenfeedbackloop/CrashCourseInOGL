cmake_minimum_required(VERSION 2.8)
project(HelloOpenGL C)

set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	"${CMAKE_SOURCE_DIR}/cmake/Modules/"
)

find_package(GLEW REQUIRED)
if(GLEW_FOUND)
  message(STATUS "Found GLEW at ${GLEW_INCLUDE_PATH} and ${GLEW_LIBRARY}")
endif(GLEW_FOUND)

#include_directories("${GLEW_INCLUDE_PATH}")

add_executable(HelloOpenGL main.c LoadShaders.h)

#target_link_libraries(HelloOpenGL "${GLEW_LIBRARY}")

set(CMAKE_EXE_LINKER_FLAGS
	"-framework OpenGL -framework GLUT -framework Cocoa"
)

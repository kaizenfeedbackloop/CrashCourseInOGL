cmake_minimum_required(VERSION 2.8)
project(Week1 CXX)

configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/shader.frag
	${CMAKE_CURRENT_BINARY_DIR}/shader.frag COPYONLY)
configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/shader.vert
	${CMAKE_CURRENT_BINARY_DIR}/shader.vert COPYONLY)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
include_directories("${GLEW_INCLUDE_PATH}")
include_directories("${GLUT_INCLUDE_PATH}")
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

add_executable(Week1 main.cpp LoadShaders.h)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
target_link_libraries(Week1 "${GLEW_LIBRARY}" ${GLUT_LIBRARY})
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(CMAKE_EXE_LINKER_FLAGS
	"-framework OpenGL -framework GLUT -framework Cocoa"
)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

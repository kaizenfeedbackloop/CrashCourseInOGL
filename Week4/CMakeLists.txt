cmake_minimum_required(VERSION 2.8)
project(Week4 CXX)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
include_directories("${GLEW_INCLUDE_PATH}")
include_directories("${GLUT_INCLUDE_PATH}")
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

include_directories("${PROJECT_SOURCE_DIR}/../include")

add_executable(Week4 main.cpp LoadShaders.h)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
target_link_libraries(Week4 "${GLEW_LIBRARY}")
target_link_libraries(Week4 "${GLUT_LIBRARY}")
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(CMAKE_EXE_LINKER_FLAGS
	"-framework OpenGL -framework GLUT -framework Cocoa"
)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

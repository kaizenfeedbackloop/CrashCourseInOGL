cmake_minimum_required(VERSION 2.8)
project(Week5 CXX)

configure_file(	${CMAKE_CURRENT_SOURCE_DIR}/Barn.png
	${CMAKE_CURRENT_BINARY_DIR}/Barn.png COPYONLY)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
include_directories("${GLEW_INCLUDE_PATH}")
include_directories("${GLUT_INCLUDE_PATH}")
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

include_directories(
"${PROJECT_SOURCE_DIR}/../include"
"${PNG_INCLUDE_PATH}"
"${ZLIB_INCLUDE_PATH}"
)

add_executable(Week5 main.cpp LoadShaders.h)

target_link_libraries(Week5 "${ZLIB_LIBRARY}")
target_link_libraries(Week5 "${PNG_LIBRARY}")

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
target_link_libraries(Week5 "${GLEW_LIBRARY}")
target_link_libraries(Week5 "${GLUT_LIBRARY}")
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(CMAKE_EXE_LINKER_FLAGS
	"-framework OpenGL -framework GLUT -framework Cocoa"
)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

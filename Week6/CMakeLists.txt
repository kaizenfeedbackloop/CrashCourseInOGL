cmake_minimum_required(VERSION 2.8)
project(Week6 CXX)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
include_directories("${GLEW_INCLUDE_PATH}")
include_directories("${GLUT_INCLUDE_DIR}")
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

include_directories(
"${PROJECT_SOURCE_DIR}/../include"
"${FREETYPE_INCLUDE_PATH}"
)

add_executable(Week6 main.cpp LoadShaders.h Font.hpp Font.cpp)

target_link_libraries(Week6 "${FREETYPE_LIBRARY}")

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
target_link_libraries(Week6 "${GLEW_LIBRARY}" ${GLUT_LIBRARY} ${GLU_LIBRARY} )
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(CMAKE_EXE_LINKER_FLAGS
	"-framework OpenGL -framework GLUT -framework Cocoa"
)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

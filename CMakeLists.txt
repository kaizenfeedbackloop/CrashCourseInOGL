cmake_minimum_required(VERSION 2.8)
project(CrashCourseInOGL CXX)

set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	"${CMAKE_SOURCE_DIR}/cmake/Modules/"
)

find_package(PNG REQUIRED)
message(STATUS "Set PNG at ${PNG_INCLUDE_PATH} and ${PNG_LIBRARY}")

find_package(ZLIB REQUIRED)
message(STATUS "Set ZLIB at ${ZLIB_INCLUDE_PATH} and ${ZLIB_LIBRARY}")

find_package(GLEW REQUIRED)
if(GLEW_FOUND)
  message(STATUS "Found GLEW at ${GLEW_INCLUDE_PATH} and ${GLEW_LIBRARY}")
else(GLEW_FOUND)
set(GLEW_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/../glew/include/")
set(GLEW_LIBRARY "${CMAKE_SOURCE_DIR}/../glew/lib/glew32.lib")
  message(STATUS "Set GLEW at ${GLEW_INCLUDE_PATH} and ${GLEW_LIBRARY}")
endif(GLEW_FOUND)

set(GLUT_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/../freeglut/include/")
set(GLUT_LIBRARY "${CMAKE_SOURCE_DIR}/../freeglut/lib/x86/freeglut.lib")

add_subdirectory(Week1)
add_subdirectory(Week2)
add_subdirectory(Week3)
add_subdirectory(Week4)
add_subdirectory(Week5)

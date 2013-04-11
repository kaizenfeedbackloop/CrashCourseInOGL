cmake_minimum_required(VERSION 2.8)
project(CrashCourseInOGL CXX)

if( ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR CMAKE_COMPILER_IS_GNUCXX )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
message(STATUS "Targeting C++11")
endif()

set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	"${CMAKE_SOURCE_DIR}/cmake/Modules/"
)

find_package(GLEW REQUIRED)
message(STATUS "Set GLEW at ${GLEW_INCLUDE_PATH} and ${GLEW_LIBRARY}")

find_package(GLUT REQUIRED)
message(STATUS "Set GLUT at ${GLUT_INCLUDE_DIR} and ${GLUT_LIBRARIES}")

find_package(GLU)
message(STATUS "Set GLU at ${GLU_INCLUDE_PATH} and ${GLU_LIBRARY}")

find_package(PNG REQUIRED)
message(STATUS "Set PNG at ${PNG_INCLUDE_PATH} and ${PNG_LIBRARY}")

find_package(ZLIB REQUIRED)
message(STATUS "Set ZLIB at ${ZLIB_INCLUDE_PATH} and ${ZLIB_LIBRARY}")

find_package(FREETYPE REQUIRED)
message(STATUS "Set FREETYPE at ${FREETYPE_INCLUDE_PATH} and ${FREETYPE_LIBRARY}")

add_subdirectory(Week1)
#add_subdirectory(Week2)
add_subdirectory(Week3)
add_subdirectory(Week4)
add_subdirectory(Week5)
add_subdirectory(Week6)

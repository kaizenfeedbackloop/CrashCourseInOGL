cmake_minimum_required(VERSION 2.8)
project(Week2 CXX)

include_directories(
	"${PROJECT_SOURCE_DIR}/../../cxxtest"
	"${PROJECT_SOURCE_DIR}/../include"
)

add_custom_command(
	OUTPUT runner.cpp
	COMMAND cxxtestgen --error-printer -o runner.cpp ${PROJECT_SOURCE_DIR}/TestSuite.hpp
	DEPENDS ${PROJECT_SOURCE_DIR}/TestSuite.hpp
)

add_executable(Week2 runner.cpp TestSuite.hpp)

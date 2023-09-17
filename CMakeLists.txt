cmake_minimum_required(VERSION 3.22)
project(xonix VERSION 0.0.1)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch godot-cpp
include(FetchContent)
FetchContent_Declare(godot-cpp
	GIT_REPOSITORY https://github.com/godotengine/godot-cpp.git
	GIT_TAG godot-4.1.1-stable)
FetchContent_MakeAvailable(godot-cpp)

# Create library
if (EMSCRIPTEN)
	add_executable(${PROJECT_NAME})
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
	# https://github.com/godotengine/godot/blob/master/platform/web/SCsub
	set_target_properties(godot-cpp PROPERTIES
		COMPILE_FLAGS "-mbulk-memory -matomics")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		COMPILE_FLAGS "-O3 -s SIDE_MODULE=2 -s SHARED_MEMORY=1 -mbulk-memory -matomics"
		LINK_FLAGS "-O3 -s SIDE_MODULE=2 -s SHARED_MEMORY=1 -mbulk-memory -matomics -s WASM_MEM_MAX=2048MB")
else ()
	add_library(${PROJECT_NAME} SHARED)
endif ()

# Add -android suffix to differentiate from Linux builds
if (ANDROID)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		OUTPUT_NAME "${PROJECT_NAME}_android")
endif ()

# Add source files
add_subdirectory(src)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# IPO / LTO
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	include(CheckIPOSupported)
	check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
	if (ipo_supported)
		set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
	else ()
		message(STATUS "LTO error: ${ipo_error}")
	endif ()
endif ()

# Link godot-cpp
target_link_libraries(${PROJECT_NAME} PRIVATE godot::cpp)

# Copy files to Godot after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy
	"$<TARGET_FILE:${PROJECT_NAME}>"
	"${CMAKE_SOURCE_DIR}/game/bin")

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
add_library(${PROJECT_NAME} SHARED)

# Add -android suffix to differentiate from Linux builds
if (ANDROID)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		OUTPUT_NAME "${PROJECT_NAME}-android")
endif ()

# Add source files
add_subdirectory(src)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Link godot-cpp
target_link_libraries(${PROJECT_NAME} PUBLIC godot::cpp)

# Copy files to Godot after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy
	"$<TARGET_FILE:${PROJECT_NAME}>"
	"${CMAKE_SOURCE_DIR}/game/bin")

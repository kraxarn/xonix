add_subdirectory(nodes)

target_sources(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
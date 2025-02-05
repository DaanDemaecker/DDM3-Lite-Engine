# Set cmake version
cmake_minimum_required(VERSION 3.11)

# Project name
project(DDM3-LITE-ENGINE)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0079 NEW)

set(SOURCE_FOLDER_NAME "DDM3-Lite")


set(EXECUTABLE_NAME "DDM3-Lite-Engine")


# Copy resources after Shaders
add_custom_target(copy_resources ALL)
add_custom_command(
    TARGET copy_resources POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/resources
    COMMAND ${CMAKE_COMMAND} -E remove
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/resources/*.vert
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/resources/*.frag
    MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/resources
    DEPENDS Shaders
)



add_custom_target(configFile ALL)
add_custom_command(
	TARGET configFile POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	-E copy
	${CMAKE_SOURCE_DIR}/Config.json
	$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
)

add_subdirectory(${SOURCE_FOLDER_NAME})
add_subdirectory(3rdParty)
add_subdirectory(Resources)
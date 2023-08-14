cmake_minimum_required(VERSION 3.10)

# Project name
project(dataStructure C CXX)

# Enable testing
enable_testing()

# Show compile command
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Set generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# set(DATA_STRUCTURE_INCLUDE_DIR )


add_subdirectory(01_seqList) 
add_subdirectory(02_linkList) 
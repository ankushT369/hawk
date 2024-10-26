# Minimum cmake version
cmake_minimum_required(VERSION 3.22.1)

# Project name
project(hawk VERSION 1.0)

# Specify the standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add include dir
include_directories(include)

# Find cJSON library
find_package(cJSON REQUIRED)

# Add the executable target
add_executable(hawk src/hawk_monitor.c src/main.c src/hawk_log.c src/hawk_parser.c)

# Link cJSON to the target
target_link_libraries(hawk cjson)


# Minimum cmake version
cmake_minimum_required(VERSION 3.22.1)

# Project name
project(hawk VERSION 0.0)


# Specify the standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add include dir
include_directories(include)

# Add the executable target
add_executable(hawk src/hawk_monitor.c src/main.c)



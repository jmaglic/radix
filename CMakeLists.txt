cmake_minimum_required(VERSION 3.20)

# Set name for project
project(radix)

include_directories(include)

# List of source files
set(SOURCES
  src/main.cpp
)

add_executable(app ${SOURCES})
set_property(TARGET app PROPERTY CXX_STANDARD 20)

cmake_minimum_required(VERSION 3.20)

# Set name for project
project(radix)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories(include)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# List of source files
set(SOURCES
  src/main.cpp
)

add_executable(app ${SOURCES})

if (BUILD_TESTING)
  include(Testing)
endif()


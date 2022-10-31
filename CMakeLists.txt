cmake_minimum_required(VERSION 3.24.2)

project(TaskSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

list(APPEND CMAKE_MESSAGE_INDENT "🔧 ")
if(UNIX)
  message(DEBUG "Configuring for UNIX")
  set(CMAKE_CXX_FLAGS -pthread)
endif()
list(POP_BACK CMAKE_MESSAGE_INDENT)

add_subdirectory(TaskSystem)
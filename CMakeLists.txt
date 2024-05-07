cmake_minimum_required(VERSION 3.20)

project(ImGuiApp C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
  message(STATUS "Use GCC/Clang")
  set(CMAKE_C_FLAGS "-g -rdynamic -fPIC")
  set(CMAKE_CXX_FLAGS "-g -rdynamic -fPIC")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_DEBUG_POSTFIX "d")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# use folders
set(USE_FOLDERS ON)

# third party libs
add_subdirectory(external)

# framework
add_subdirectory(framework)

# resources
add_subdirectory(resources)

# custom source
add_subdirectory(src)

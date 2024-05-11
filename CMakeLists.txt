cmake_minimum_required(VERSION 3.20)

project(ImGuiApp C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 设置一些编译器标志/选项
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message(STATUS "using Clang")
  set(COMMON_FLAGS "-Wall -Wextra -g3 -fvisibility=hidden -fPIC -Werror=return-type -Wno-unused-parameter")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} ${COMMON_FLAGS} -O0")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} -O0")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
  set(LINK_FLAGS_RELEASE -s)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  message(STATUS "using GNU")
  set(COMMON_FLAGS "-Wall -Wextra -g3 -fvisibility=hidden -fPIC -Werror=return-type -Wno-unused-parameter")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} ${COMMON_FLAGS} -O0")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} -O0")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
  set(LINK_FLAGS_RELEASE -s)
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.0)
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -fconcepts -fcoroutines")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fconcepts -fcoroutines")
  endif ()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  message(STATUS "using MSVC")
  # 启用文件夹
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Only Support VS2022
  if (MSVC_VERSION LESS 1930)
      message(FATAL_ERROR "Only VS2022 Support!!")
  endif()

  # Release Mode also generate PDB
  add_compile_options("$<$<NOT:$<CONFIG:Debug>>:/Zi>")
  add_link_options("$<$<NOT:$<CONFIG:Debug>>:/DEBUG>")
  add_link_options("$<$<NOT:$<CONFIG:Debug>>:/OPT:REF>")
  add_link_options("$<$<NOT:$<CONFIG:Debug>>:/OPT:ICF>")

  add_compile_options(/utf-8 /MP /Zi)
  add_definitions(-D_WIN32_WINNT=0x0601 -D_CRT_SECURE_NO_WARNINGS)
  add_link_options(/NODEFAULTLIB:LIBCMTD /NODEFAULTLIB:LIBCMT)
endif()

if (WIN32)
  if (NOT DEFINED VCPKG_PACKAGE_DIR)
    message(FATAL_ERROR "Must define VCPKG_PACKAGE_DIR, USE -DVCPKG_PACKAGE_DIR=<vcpkg package install directory>")
  endif()
  set(ENV{PATH} "${CMAKE_CURRENT_SOURCE_DIR}/win-tools/pkg-config-lite/bin;$ENV{PATH}")
  set(ENV{PKG_CONFIG_PATH} "${PKG_CONFIG_PATH};${VCPKG_PACKAGE_DIR}/lib/pkgconfig")
endif()

# 使用 PkgConfig 来查找软件包
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)

# 使用 freetype2
pkg_check_modules(ft2 REQUIRED IMPORTED_TARGET freetype2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_DEBUG_POSTFIX "d")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# use folders
set(USE_FOLDERS ON)

option(FW_BUILD_SHARED_LIBS "Build Shared Library" ON)

# third party libs
add_subdirectory(external)

# framework
add_subdirectory(src)

# resources
add_subdirectory(resources)

# example
add_subdirectory(example)

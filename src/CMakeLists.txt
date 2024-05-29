set(CMAKE_FOLDER "Framework")
project(Framework CXX)

file(GLOB_RECURSE FK_SRCS *.*)
if (FW_BUILD_SHARED_LIBS)
  add_library(${PROJECT_NAME} SHARED ${FK_SRCS})
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DFW_BUILD)
else()
  add_library(${PROJECT_NAME} STATIC ${FK_SRCS})
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DFW_STATIC)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL cpr::cpr)
target_link_libraries(${PROJECT_NAME} PUBLIC ImGui ImPlot ImNodeEd fmt)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# make tree
source_group(
  TREE
  ${CMAKE_CURRENT_SOURCE_DIR}
  FILES ${FK_SRCS}
)

# backward-cpp dependencies
if (UNIX)
  pkg_check_modules(libunwind REQUIRED IMPORTED_TARGET libunwind)
  pkg_check_modules(libdw REQUIRED IMPORTED_TARGET libdw)
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DBACKWARD_HAS_LIBUNWIND=1 -DBACKWARD_HAS_DW=1)
  target_link_libraries(${PROJECT_NAME} PUBLIC PkgConfig::libunwind PkgConfig::libdw)
endif()


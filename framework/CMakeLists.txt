set(CMAKE_FOLDER "Framework")
project(Framework CXX)

# we need opengl
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE FK_SRCS *.*)
add_library(${PROJECT_NAME} STATIC ${FK_SRCS})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw ImGui ImPlot OpenGL::OpenGL fmt)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

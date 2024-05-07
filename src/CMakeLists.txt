set(CMAKE_FOLDER "Application")
project(App C CXX)

file(GLOB_RECURSE APP_SRCS *.*)

add_executable(${PROJECT_NAME} ${APP_SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE Framework)
add_dependencies(${PROJECT_NAME} Assets)

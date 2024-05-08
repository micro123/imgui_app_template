set(CMAKE_FOLDER "Application")
project(App C CXX)

file(GLOB_RECURSE APP_SRCS *.*)

add_executable(${PROJECT_NAME} ${APP_SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE Framework)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

get_target_property(
  ASSETS_ROOT_DIR Assets RESOURCE_DIR
)
message(STATUS "ASSETS ROOT: ${ASSETS_ROOT_DIR}")

add_custom_target(
  COPY_APP_RESOURCES ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_ROOT_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

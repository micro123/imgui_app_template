set(CMAKE_FOLDER "Third Party")
project(third_party_libs)

# glfw
set(GLFW_LIBRARY_TYPE "STATIC")
add_subdirectory(glfw)

# imgui
set(
  IMGUI_FILES
  imgui/imgui_demo.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_internal.h
  imgui/imgui_tables.cpp
  imgui/imgui_widgets.cpp
  imgui/imgui.cpp
  imgui/imgui.h
  imgui/imstb_rectpack.h
  imgui/imstb_textedit.h
  imgui/imstb_truetype.h
  imgui/backends/imgui_impl_opengl3.h
  imgui/backends/imgui_impl_opengl3.cpp
  imgui/backends/imgui_impl_opengl3_loader.h
  imgui/backends/imgui_impl_glfw.h
  imgui/backends/imgui_impl_glfw.cpp
  imgui/misc/cpp/imgui_stdlib.h
  imgui/misc/cpp/imgui_stdlib.cpp
  imgui/misc/freetype/imgui_freetype.h
  imgui/misc/freetype/imgui_freetype.cpp
)
add_library(ImGui OBJECT ${IMGUI_FILES})
target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_link_libraries(ImGui PUBLIC PkgConfig::ft2 glfw)
target_compile_definitions(ImGui PUBLIC -DIMGUI_USER_CONFIG="imgui_config_user.h")
if (FW_BUILD_SHARED_LIBS)
  target_compile_definitions(ImGui PUBLIC -DFW_BUILD)
endif()

source_group(
  TREE
  ${CMAKE_CURRENT_SOURCE_DIR}
  FILES ${IMGUI_FILES}
)

# implot
set(
  IMPLOT_FILES
  implot/implot_demo.cpp
  implot/implot_internal.h
  implot/implot_items.cpp
  implot/implot.cpp
  implot/implot.h
)
add_library(ImPlot OBJECT ${IMPLOT_FILES})
target_include_directories(ImPlot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/implot)
target_link_libraries(ImPlot PUBLIC ImGui)

source_group(
  TREE
  ${CMAKE_CURRENT_SOURCE_DIR}
  FILES ${IMPLOT_FILES}
)

# fmt
add_subdirectory(fmt)

# cpr
set(CPR_USE_SYSTEM_CURL ON)
set(CPR_ENABLE_CURL_HTTP_ONLY OFF)

if (WIN32)
  set(CPR_USE_SYSTEM_CURL OFF)
endif()

add_subdirectory(cpr)

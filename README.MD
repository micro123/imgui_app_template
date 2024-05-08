# ImGui App Template

A template project for `ImGui`.

## Features

- [x] CMake Project
- [x] C++ 20
- [ ] Compiler Support
  - [x] GCC / Clang on Linux
  - [x] MSVC on Windows (vs2022+, need vcpkg)
- [x] GLFW
- [x] FMT
- [x] ImGui
  - [x] ImplGlfw
  - [x] ImplOpenGL3
- [x] ImPlot
- [x] Customize
  - [x] ImGui User Config Header `include/imgui_config_user.h`
  - [x] ImGui Config
  - [x] Window Title
  - [x] Window Size
  - [x] Style (ImGuiStyle)
  - [x] Fonts (FreeType)
  - [x] Window Content
  - [x] User Components

## Getting Start

```bash
git clone --recursive <repo_url> <some_dir>
cd <some_dir>
cmake -B build .
cmake --build build
```

## Example Code

See `src/entry_point.cpp`, just implement all the functions which declared in file `framework/app/entry_point.hpp`. User code should place under the `src` folder, application resources should place under the `resources/root` folder.


## ToDo List

- [x] Windows MSVC Support
- [x] Convert to OOP-Style
- [ ] And may convert an shared library?


## Credists

Some code use opensource libraries, incomplete list:
- [imgui](https://github.com/ocornut/imgui.git)
- [implot](https://github.com/epezent/implot.git)
- [glfw](https://github.com/glfw/glfw.git)
- [fmt](https://github.com/fmtlib/fmt.git)
- [subprocess](https://github.com/arun11299/cpp-subprocess)
- [json](https://github.com/nlohmann/json)
- [pugixml](https://pugixml.org/)

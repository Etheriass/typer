cmake_minimum_required(VERSION 3.31)
project(typer_sdl3 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(TT_BUILD_RELEASE "Build with -O3 -DNDEBUG" ON)
if(TT_BUILD_RELEASE)
  add_compile_options(-O3 -DNDEBUG)
endif()

# Find SDL3 + SDL3_ttf
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

add_subdirectory(src)


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
# add_executable(typer src/main.cpp)
# target_link_libraries(typer PRIVATE SDL3::SDL3)

# cmake_minimum_required(VERSION 3.16)
# project(offline_typetest_sdl2 LANGUAGES CXX)
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Options
# option(TT_BUILD_RELEASE "Build with -O3 -DNDEBUG" ON)
# if(TT_BUILD_RELEASE)
#   add_compile_options(-O3 -DNDEBUG)
# endif()

# # Find SDL2 + SDL2_ttf
# find_package(SDL2 REQUIRED CONFIG)
# find_package(SDL2_ttf REQUIRED CONFIG)

# add_executable(typetest
#   src/main.cpp
# )

# target_link_libraries(typetest
#   PRIVATE
#     SDL2::SDL2
#     SDL2_ttf::SDL2_ttf
# )

# # Runtime working dir (for assets)
# add_custom_target(copy_assets ALL
#   COMMAND ${CMAKE_COMMAND} -E copy_directory
#           ${CMAKE_SOURCE_DIR}/assets
#           ${CMAKE_BINARY_DIR}/assets
# )
# add_dependencies(typetest copy_assets)

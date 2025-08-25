add_library(typer_lib
    word_run.cpp
    word_run.h
    text_displayer.cpp
    text_displayer.h
    colors.h
)

add_executable(typer_main main.cpp)
target_link_libraries(typer_main PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf typer_lib)


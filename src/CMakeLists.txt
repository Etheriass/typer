add_library(typer_lib
    word.cpp
    word.h
    text.cpp
    text.h
    displayer.cpp
    displayer.h
    colors.h
    stats.h
)

add_executable(typer_main main.cpp)
target_link_libraries(typer_main PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf typer_lib)

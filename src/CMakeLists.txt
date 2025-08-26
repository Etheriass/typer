add_library(typer_lib
    word.cpp
    word.h
    vocabulary.cpp
    vocabulary.h
    displayer.h
    colors.h
    stats.h
    session.h
    const.h
    layout.h
)

add_executable(typer_main main.cpp)
target_link_libraries(typer_main PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf typer_lib)

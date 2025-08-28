add_library(typer_lib
    text.cpp
    text.h
    vocabulary.cpp
    vocabulary.h
    displayer.h
    theme.h
    stats.h
    session.h
    const.h
    layout.h
)

add_executable(typer_main main.cpp)
target_link_libraries(typer_main PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf typer_lib)

# Copy assets folder
add_custom_command(TARGET typer_main POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:typer_main>/assets)

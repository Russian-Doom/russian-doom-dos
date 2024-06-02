add_library(audio_lib INTERFACE
    pcfx.h
)

target_include_directories(audio_lib INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

set_source_files_properties(pcfx.obj PROPERTIES
    EXTERNAL_OBJECT true
    GENERATED true
)

target_link_libraries(audio_lib INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/audio_wf.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/pcfx.obj"
)

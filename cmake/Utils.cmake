# add_copy_target(<target_name> SOURCE <file> ... [DESTINATION <dir>])
# Copy given files to given dir at build time
function(add_copy_target target_name)
    cmake_parse_arguments(PARSE_ARGV 1 "ARG"
        ""            # List of Options
        "DESTINATION" # List of Values
        "SOURCE"      # List of Lists
    )

    if(NOT ARG_DESTINATION)
        set(ARG_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
    endif()

    set(output_dir "${ARG_DESTINATION}")

    foreach(source_file ${ARG_SOURCE})
        add_custom_command(OUTPUT "${output_dir}/${source_file}"
            MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/${source_file}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/${source_file}"
            "${output_dir}/${source_file}"
            COMMENT "Copying ${output_dir}/${source_file}"
        )
        list(APPEND destination_files "${output_dir}/${source_file}")
    endforeach()

    add_custom_target(${target_name} DEPENDS ${destination_files})
    set_target_properties(${target_name} PROPERTIES
        OUT_FILES "${destination_files}"
    )
endfunction()

# configure_empty_git_info(<Template> <Output>)
function(configure_empty_git_info Template Output)
    set(Hash "<unknown>")
    set(Timestamp "<unknown>")
    set(Version_suffix "")
    set(Display_version_suffix "")
    configure_file("${Template}" "${Output}" @ONLY)
endfunction()

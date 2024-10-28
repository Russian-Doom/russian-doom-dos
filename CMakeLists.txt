cmake_minimum_required(VERSION 3.0...3.29)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project("Russian Doom DOS" VERSION 1.8 LANGUAGES C NONE)

include(CMakeDependentOption)
include(Utils)

# Git info options
find_package(Git)
cmake_dependent_option(RD_GIT_NO_HASH "For Devs only! Don't run 'revision_check' at all" OFF "GIT_FOUND" ON)

# Update git_info.h
if(RD_GIT_NO_HASH)
    # Use a function to avoid accidentally messing the real variables
    configure_empty_git_info("cmake/git_info.h.in" "git_info.h")
else()
    add_custom_target(revision_check
        COMMAND "${CMAKE_COMMAND}" -P "\"${PROJECT_SOURCE_DIR}/cmake/UpdateRevision.cmake\"" "${GIT_EXECUTABLE}" "\"git_info.h\""
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
endif()

# Configure defines header (Cmake's target_compile_definitions not working)
configure_file("cmake/config.h.in" "config.h")

# Sources
add_subdirectory(audio)
add_subdirectory(src)

# CPack
set(CPACK_PACKAGE_NAME "russian-doom-dos")
set(CPACK_SYSTEM_NAME "i386")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CHECKSUM SHA256)
set(CPACK_GENERATOR ZIP)

# Archive Generator
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_ARCHIVE_THREADS 0) # For Cmake 3.18+

include(CPack)

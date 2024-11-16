cmake_minimum_required(VERSION 3.0...3.29)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project("Russian Doom DOS" VERSION 1.8 LANGUAGES C NONE)

include(CMakeDependentOption)
include(Utils)
include(VersionFromGit)

# Configure defines header (Cmake's target_compile_definitions not working)
configure_file("cmake/config.h.in" "config.h")

# Sources
add_subdirectory(wads)
add_subdirectory(audio)
add_subdirectory(src)

# CPack
set(CPACK_PACKAGE_NAME "russian-doom-dos")
set(CPACK_SYSTEM_NAME "i386")
set(CPACK_PACKAGE_CHECKSUM SHA256)
set(CPACK_GENERATOR ZIP)
set(CPACK_PROJECT_CONFIG_FILE "CPackProjectConfig.cmake")

# Archive Generator
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_ARCHIVE_THREADS 0) # For Cmake 3.18+

include(CPack)

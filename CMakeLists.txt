cmake_minimum_required(VERSION 3.0...3.29)

project("Russian Doom DOS" VERSION 1.9 LANGUAGES C NONE)

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

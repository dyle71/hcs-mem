# ------------------------------------------------------------
# This file defines the CMake DEB package creation for mem
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2020 headcode.space
# https://www.headcode.space, <info@headcode.space>
# ------------------------------------------------------------

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_SOURCE_DIR}/cmake/cpack/deb/control/postinst;\
                                        ${CMAKE_SOURCE_DIR}/cmake/cpack/deb/control/postrm;\
                                        ${CMAKE_SOURCE_DIR}/cmake/cpack/deb/control/prerm;")
string(TOLOWER "${CPACK_PACKAGE_NAME}" CPACK_PACKAGE_NAME_LOWERCASE)
set(CPACK_PROJECT_VERSION_STRING "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
find_program(DPKG_PROGRAM dpkg DOC "dpkg program of Debian-based systems")
if (DPKG_PROGRAM)
    # use dpkg to fix the package file name
    execute_process(
            COMMAND ${DPKG_PROGRAM} --print-architecture
            OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME_LOWERCASE}_${CPACK_PROJECT_VERSION_STRING}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
    set(PACKAGE_ARCH ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE})
else ()
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME_LOWERCASE}_${CPACK_PROJECT_VERSION_STRING}_${CMAKE_SYSTEM_NAME}")
    set(PACKAGE_ARCH ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif ()
message(STATUS "Package filename: ${CPACK_PACKAGE_FILE_NAME}.deb.")


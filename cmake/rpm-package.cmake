# ------------------------------------------------------------
# This file defines the CMake RPM package creation for mem
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2020 headcode.space
# https://www.headcode.space, <info@headcode.space>
# ------------------------------------------------------------

set(CPACK_GENERATOR "RPM")
set(CPACK_RPM_PACKAGE_DEPENDS "")

set(CPACK_RPM_PRE_INSTALL_SCRIPT_FILE "${CMAKE_SOURCE_DIR}/cmake/cpack/rpm/preinst")
set(CPACK_RPM_PRE_UNINSTALL_SCRIPT_FILE  "${CMAKE_SOURCE_DIR}/cmake/cpack/rpm/uinstall")
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${CMAKE_SOURCE_DIR}/cmake/cpack/rpm/postinst")

string(TOLOWER "${CPACK_PACKAGE_NAME}" CPACK_PACKAGE_NAME_LOWERCASE)
set(CPACK_PROJECT_VERSION_STRING "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME_LOWERCASE}_${CPACK_PROJECT_VERSION_STRING}_${CMAKE_SYSTEM_NAME}")
set(PACKAGE_ARCH ${CMAKE_HOST_SYSTEM_PROCESSOR})
message(STATUS "Package filename: ${CPACK_PACKAGE_FILE_NAME}.deb.")


#.rst:
# FindJPEG
# --------
#
# Find JPEG
#
# Find the native JPEG includes and library This module defines
#
# ::
#
#   JPEG_INCLUDE_DIR, where to find jpeglib.h, etc.
#   JPEG_LIBRARIES, the libraries needed to use JPEG.
#   JPEG_FOUND, If false, do not try to use JPEG.
#
# also defined, but not for general use are
#
# ::
#
#   JPEG_LIBRARY, where to find the JPEG library.

#=============================================================================
# Copyright 2001-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

find_path(LibJpegTurbo_INCLUDE_DIR jpeglib.h PATHS "/opt/libjpeg-turbo/include/" NO_DEFAULT_PATH)
find_path(LibJpegTurbo_INCLUDE_DIR jpeglib.h)

set(LibJpegTurbo_NAMES ${LibJpegTurbo_NAMES} turbojpeg-static jpeg-static libjpeg jpeg )
find_library(LibJpegTurbo_LIBRARY NAMES ${LibJpegTurbo_NAMES} PATHS "/opt/libjpeg-turbo/lib64/" NO_DEFAULT_PATH)
find_library(LibJpegTurbo_LIBRARY NAMES ${LibJpegTurbo_NAMES} )

# handle the QUIETLY and REQUIRED arguments and set JPEG_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibJpegTurbo DEFAULT_MSG LibJpegTurbo_LIBRARY LibJpegTurbo_INCLUDE_DIR)

if(LibJpegTurbo_FOUND)
  set(LibJpegTurbo_LIBRARIES ${LibJpegTurbo_LIBRARY})
endif()


mark_as_advanced(LibJpegTurbo_LIBRARY LibJpegTurbo_INCLUDE_DIR )
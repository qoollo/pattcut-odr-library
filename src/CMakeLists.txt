cmake_minimum_required (VERSION 2.8)
project (RecognitionTool)

# ============ common configurations ==========

set(PLATFORM_BITNESS_NAME "x86")
if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
	message(STATUS "x86 detected")
	set(PLATFORM_BITNESS_NAME "x86")
else()
	message(STATUS "x64 detected")
	set(PLATFORM_BITNESS_NAME "x64")
endif()


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
if(WIN32)
	set(CUSTOM_CMAKE_PREFIX_PATH
		"${CMAKE_CURRENT_LIST_DIR}/libs/libjpeg-turbo"
		"${CMAKE_CURRENT_LIST_DIR}/libs/libpng"
		"${CMAKE_CURRENT_LIST_DIR}/libs/zlib"
		"${CMAKE_CURRENT_LIST_DIR}/libs/ImageMagick"
	)

	foreach (CUR_PATH ${CUSTOM_CMAKE_PREFIX_PATH})
		list(APPEND CMAKE_PREFIX_PATH "${CUR_PATH}")
		list(APPEND CMAKE_PREFIX_PATH "${CUR_PATH}/${PLATFORM_BITNESS_NAME}")
	endforeach()
endif(WIN32)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/exe)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin/module)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin/lib)

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
    add_definitions(-WX)
endif(MSVC)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
	add_definitions(-std=c++11)
	add_definitions(-Wall)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_definitions(-std=c++11)
	add_definitions(-Werror)
endif()



if(MSVC)
	#We statically link to reduce dependancies
	foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		if(${flag_var} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
		endif(${flag_var} MATCHES "/MD")
		if(${flag_var} MATCHES "/MDd")
			string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
		endif(${flag_var} MATCHES "/MDd")
	endforeach(flag_var)
	
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL /Ox")
endif(MSVC)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto")
endif()


# ============= main targets ===========

add_subdirectory(RecognitionLib)
add_subdirectory(RecognitionLibTest)


add_custom_target(
	build_all
	DEPENDS RecognitionLib RecognitionLibTest
)

set_property(TARGET build_all PROPERTY FOLDER "Helpers")

# === distclean target ============                  
ADD_CUSTOM_TARGET(distclean 
    COMMENT "Cleaning for source distribution")
	
set_property(TARGET distclean PROPERTY FOLDER "Helpers")

if (${CMAKE_GENERATOR} MATCHES Makefile)
	add_custom_command (TARGET distclean
						COMMAND make clean
	)
endif (${CMAKE_GENERATOR} MATCHES Makefile)

SET(DISTCLEANED_FILES
   ${DISTCLEANED_FILES}
   cmake.depends
   cmake.check_depends
   CMakeCache.txt
   cmake.check_cache
   Makefile
   *~
)

SET(DISTCLEANED_DIRS
   ${DISTCLEANED_DIRS}
   CMakeFiles
)

SET(DISTCLEANED_NOREC
    ${DISTCLEANED_NOREC}
    ${CMAKE_SOURCE_DIR}/install
    ${CMAKE_SOURCE_DIR}/install_manifest.txt
    ${CMAKE_SOURCE_DIR}/src/bin
)

# Для Windows не надо. Там работаем через Visual Studio
if (UNIX)
	FOREACH (DSTCLEANELEM ${DISTCLEANED_DIRS})
	   ADD_CUSTOM_COMMAND(
	    COMMAND find ${CMAKE_BINARY_DIR} -type d -name \"${DSTCLEANELEM}\" -print0 | xargs -0 rm -rf
	    TARGET  distclean
	  )
	ENDFOREACH (DSTCLEANELEM)

	FOREACH (DSTCLEANELEM ${DISTCLEANED_FILES})
	   ADD_CUSTOM_COMMAND(
	    COMMAND find ${CMAKE_BINARY_DIR} -type f -name \"${DSTCLEANELEM}\" -delete
	    TARGET  distclean
	  )
	ENDFOREACH (DSTCLEANELEM)

	FOREACH (DSTCLEANELEM ${DISTCLEANED_NOREC})
	   ADD_CUSTOM_COMMAND(
	    COMMAND rm -rf ${DSTCLEANELEM}
	    TARGET  distclean
	  )
	ENDFOREACH (DSTCLEANELEM)
endif(UNIX)

  
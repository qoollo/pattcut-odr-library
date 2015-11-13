cmake_minimum_required (VERSION 2.8)
project (RecognitionLibTest)


set(SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

set(HEADER_FILES
)


include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../RecognitionLib/inc/export/)


add_executable(RecognitionLibTest ${SOURCE_FILES} ${HEADER_FILES})
add_dependencies(RecognitionLibTest RecognitionLib)
add_custom_command(TARGET RecognitionLibTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/RecognitionLib/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

# В таком виде не работает
#target_link_libraries(RecognitionLibTest
#					  ${CMAKE_CURRENT_SOURCE_DIR}/../RecognitionLib/bin/lib/${CMAKE_SHARED_LIBRARY_PREFIX}RecognitionLib${CMAKE_SHARED_LIBRARY_SUFFIX})


target_link_libraries(RecognitionLibTest RecognitionLib)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
	find_package(Threads)
	target_link_libraries(RecognitionLibTest ${CMAKE_THREAD_LIBS_INIT})
endif()


add_custom_target(build_test DEPENDS RecognitionLibTest)
set_property(TARGET build_test PROPERTY FOLDER "Helpers")
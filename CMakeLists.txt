
cmake_minimum_required( VERSION 3.26.3 )

set( APP_NAME to_png )

project(
	${APP_NAME}
	VERSION 1.0
	LANGUAGES C
)

add_executable( app src/main.c )

target_compile_definitions( app PRIVATE "$<$<CONFIG:Debug>:DEBUG>$<$<CONFIG:Release>:NDEBUG>" )
target_compile_definitions( app PRIVATE -DBUILD_TYPE="$<$<CONFIG:Debug>:DEBUG>$<$<CONFIG:Release>:RELEASE>" )

target_include_directories( app PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/" )

if ( MSVC )
	target_compile_options( app PRIVATE -WX -W4 -D_CRT_SECURE_NO_WARNINGS )
	target_compile_options( app PRIVATE $<$<CONFIG:Debug>:-Z7 -FC> )
	target_compile_options( app PRIVATE $<$<CONFIG:Release>:-O2 -Ot -GF> )
endif()

if ( CMAKE_COMPILER_IS_GNUCC )
	target_compile_options( app PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-uninitialized )
	target_compile_options( app PRIVATE $<$<CONFIG:Debug>:-OO -g> )
	target_compile_options( app PRIVATE $<$<CONFIG:Release>:-O2> )
endif()

set( outputDirectory "${CMAKE_CURRENT_SOURCE_DIR}/TEMP/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>" )

set_target_properties(
	app
	PROPERTIES
	OUTPUT_NAME                      "${APP_NAME}"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${outputDirectory}/"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${outputDirectory}/"
)

add_custom_command(
	TARGET app
	PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E "$<IF:$<CONFIG:Debug>,remove_directory,true>"
		"$<$<CONFIG:Debug>:${outputDirectory}/logs/>"
	COMMAND ${CMAKE_COMMAND} -E "$<IF:$<CONFIG:Release>,remove,true>"
		"${outputDirectory}/${APP_NAME}.pdb"
	COMMAND_EXPAND_LISTS
)

add_custom_command(
	TARGET app
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E "$<IF:$<CONFIG:Release>,tar,true>"
		cvf ${CMAKE_CURRENT_SOURCE_DIR}/${APP_NAME}.zip --format=zip .
	WORKING_DIRECTORY ${outputDirectory}
	COMMAND_EXPAND_LISTS
)
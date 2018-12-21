cmake_minimum_required(VERSION 3.1)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
		STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

project(liboco)

set(${PROJECT_NAME}_EXECUTABLES
	rfc862c
	rfc862d
	)

add_executable(open_fds open_fds.cc)
install(TARGETS open_fds RUNTIME DESTINATION bin)

set(${PROJECT_NAME}_OBJECTS
	acceptor
	connector
	context
	core
	counter
	demangler
	duration
	file_descriptor
	inet6_address
	inet_address
	killable
	killer
	local_address
	reactor
	scheduler
	semaphore
	signaler
	signal_killer
	signal_set
	signal_stack
	signal_suppressor
	simple_task
	socket_address
	stack
	task
	timer
	trigger
	)

set(${PROJECT_NAME}_HEADERS
	resource_limit
	)
list(APPEND ${PROJECT_NAME}_HEADERS "${${PROJECT_NAME}_OBJECTS}")

set(${PROJECT_NAME}_TESTS
	test_semaphore
	test_timers
	)

add_library(oco OBJECT)
foreach(object ${${PROJECT_NAME}_OBJECTS})
	target_sources(oco PUBLIC ${object}.cc)
endforeach(object)
set_property(TARGET oco PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(oco-static STATIC $<TARGET_OBJECTS:oco>)
set_target_properties(oco-static PROPERTIES OUTPUT_NAME oco)
add_library(oco-shared SHARED $<TARGET_OBJECTS:oco>)
set_target_properties(oco-shared PROPERTIES OUTPUT_NAME oco SOVERSION 0 VERSION 0.0.0)

install(TARGETS
	oco-static
	oco-shared
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	)

foreach(executable ${${PROJECT_NAME}_EXECUTABLES})
	add_executable(${executable} ${executable}.cc)
	target_link_libraries(${executable} oco-shared)
	install(TARGETS ${executable} RUNTIME DESTINATION bin)
endforeach(executable)

foreach(header ${${PROJECT_NAME}_HEADERS})
	install(FILES ${header}.h DESTINATION "include/loco")
endforeach(header)

enable_testing()
add_custom_target(all_tests)
foreach(test ${${PROJECT_NAME}_TESTS})
	add_executable(${test} EXCLUDE_FROM_ALL ${test}.cc)
	target_link_libraries(${test} oco-static)
	add_test(NAME ${test} COMMAND $<TARGET_FILE:${test}>)
	add_dependencies(all_tests ${test})
endforeach(test)
build_command(CTEST_CUSTOM_PRE_TEST TARGET all_tests)
string(CONFIGURE \"@CTEST_CUSTOM_PRE_TEST@\" CTEST_CUSTOM_PRE_TEST_QUOTED ESCAPE_QUOTES)
file(WRITE "${CMAKE_BINARY_DIR}/CTestCustom.cmake" "set(CTEST_CUSTOM_PRE_TEST ${CTEST_CUSTOM_PRE_TEST_QUOTED})" "\n")

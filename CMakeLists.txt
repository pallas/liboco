cmake_minimum_required(VERSION 3.1)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
		STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

project(liboco)

add_library(oco OBJECT
	acceptor.cc
	connector.cc
	context.cc
	core.cc
	counter.cc
	demangler.cc
	duration.cc
	file_descriptor.cc
	inet6_address.cc
	inet_address.cc
	killable.cc
	killer.cc
	local_address.cc
	reactor.cc
	scheduler.cc
	semaphore.cc
	signaler.cc
	signal_killer.cc
	signal_set.cc
	signal_stack.cc
	signal_suppressor.cc
	simple_task.cc
	socket_address.cc
	stack.cc
	task.cc
	timer.cc
	trigger.cc
	)
set_property(TARGET oco PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(oco-static STATIC $<TARGET_OBJECTS:oco>)
set_target_properties(oco-static PROPERTIES OUTPUT_NAME oco)
add_library(oco-shared SHARED $<TARGET_OBJECTS:oco>)
set_target_properties(oco-shared PROPERTIES OUTPUT_NAME oco SOVERSION 0 VERSION 0.0.0)

add_executable(open_fds open_fds.cc)
add_executable(rfc862c rfc862c.cc)
target_link_libraries(rfc862c oco-static)
add_executable(rfc862d rfc862d.cc)
target_link_libraries(rfc862d oco-static)

file(GLOB header_files "*.h")
install(FILES ${header_files}
	DESTINATION "include/loco"
	)

install(TARGETS
	oco-shared
	oco-static
	open_fds
	rfc862c
	rfc862d
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	)

enable_testing()
add_executable(test_semaphore test_semaphore.cc)
target_link_libraries(test_semaphore oco-static)
add_test(NAME test_semaphore COMMAND $<TARGET_FILE:test_semaphore>)
add_executable(test_timers test_timers.cc)
target_link_libraries(test_timers oco-static)
add_test(NAME test_timers COMMAND $<TARGET_FILE:test_timers>)

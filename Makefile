default: all

DEBUG ?= -g -ggdb -DDEBUG
ifeq ($(DEBUG),)
	override DEBUG := -DNDEBUG -O2
endif

override LDFLAGS += -lstdc++
override CFLAGS += $(DEBUG) -MD -MP
override CXXFLAGS += $(DEBUG) -MD -MP

PROGRAMS := \
	open_fds \
	rfc862c \
	rfc862d \
	#

TESTS := \
	test_semaphore \
	test_timers \
	#

COMMON := \
	acceptor \
	connector \
	context \
	core \
	counter \
	demangler \
	duration \
	file_descriptor \
	inet6_address \
	inet_address \
	killable \
	killer \
	local_address \
	reactor \
	scheduler \
	semaphore \
	signaler \
	signal_killer \
	signal_set \
	signal_stack \
	signal_suppressor \
	simple_task \
	socket_address \
	stack \
	task \
	timer \
	trigger \
	#

DEPENDS = $(PROGRAMS:=.d) $(TESTS:=.d) $(COMMON:=.d)
-include $(DEPENDS)

OBJECTS = $(PROGRAMS:=.o) $(TESTS:=.o) $(COMMON:=.o)

$(PROGRAMS) $(TESTS) : %: %.o $(COMMON:=.o)

all: $(PROGRAMS)

RUN_TESTS := $(addprefix run/,$(TESTS))

.PHONY: test
test: $(RUN_TESTS)

.PHONY: $(RUN_TESTS)
$(RUN_TESTS) : run/%: %
	./$<

.PHONY: clean
clean:
	rm -rf $(PROGRAMS) $(TESTS) $(OBJECTS) $(DEPENDS)

#

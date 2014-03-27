default: all

DEBUG ?= -g -ggdb -DDEBUG
ifeq ($(DEBUG),)
	override DEBUG := -DNDEBUG
endif

override LDFLAGS += -lstdc++
override CFLAGS += $(DEBUG) -MD -MP
override CXXFLAGS += $(DEBUG) -MD -MP

PROGRAMS := \
	open_fds \
	#

COMMON := \
	acceptor \
	connector \
	context \
	core \
	demangler \
	file_descriptor \
	inet6_address \
	inet_address \
	killable \
	killer \
	local_address \
	reactor \
	scheduler \
	signal_set \
	signal_suppressor \
	simple_task \
	socket_address \
	stack \
	task \
	trigger \
	#

DEPENDS = $(PROGRAMS:=.d) $(COMMON:=.d)
-include $(DEPENDS)

OBJECTS = $(PROGRAMS:=.o) $(COMMON:=.o)

$(PROGRAMS) : %: %.o

all: $(PROGRAMS) $(OBJECTS)

.PHONY: clean
clean:
	rm -rf $(PROGRAMS) $(OBJECTS) $(DEPENDS)

#

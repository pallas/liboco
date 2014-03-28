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
	context \
	core \
	demangler \
	file_descriptor \
	killable \
	killer \
	reactor \
	scheduler \
	signal_set \
	signal_suppressor \
	simple_task \
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

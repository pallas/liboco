default: all

DEBUG ?= -g -ggdb -DDEBUG
ifeq ($(DEBUG),)
	override DEBUG := -DNDEBUG
endif

override LDFLAGS += -lstdc++
override CFLAGS += $(DEBUG) -MD -MP
override CXXFLAGS += $(DEBUG) -MD -MP

COMMON := \
	context \
	demangler \
	scheduler \
	stack \
	task \
	#

DEPENDS = $(COMMON:=.d)
-include $(DEPENDS)

OBJECTS = $(COMMON:=.o)

all: $(OBJECTS)

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(DEPENDS)

#

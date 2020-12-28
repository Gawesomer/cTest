UTILDIR    := ./util
INCLUDEDIR := ./include

CC	 := gcc
CFLAGS	 := -std=gnu11 -g -ggdb3 -Wall -Wextra -Werror -pedantic
CPPFLAGS := -I $(INCLUDEDIR) -MMD -MP
ARFLAGS  := rvu
RM	 := rm -rf

SRCS := $(shell find . -name "*.c")
OBJS := $(SRCS:%.c=%.o)
DEPS := $(OBJS:%.o=%.d)
DSYM := $(shell find . -name "*.dSYM")

# each module will add to these
PROGRAMS :=
TESTS	 :=
PHONYS   :=

include $(shell find . -name "module.mk")

-include $(DEPS)

all: $(PROGRAMS) $(TESTS)

tests: $(TESTS)

clean:
	$(RM) $(PROGRAMS) $(TESTS) $(OBJS) $(DEPS) $(DSYM)

.PHONY: all tests clean $(PHONYS)

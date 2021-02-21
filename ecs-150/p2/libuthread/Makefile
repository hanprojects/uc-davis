# Target library
lib := libuthread.a
objs := queue.o uthread.o context.o preempt.o

# Flags
CC := gcc
CFLAGS := -Wall -Wextra -Werror -MMD
CFLAGS += -g

# Source: https://tldp.org/HOWTO/Program-Library-HOWTO/static-libraries.html
AR := ar
AFLAGS := rcs

ifneq ($(V),1)
Q = @
endif

all: $(lib)

# Dependency tracking (below `all` rule)
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

libuthread.a: $(objs)
	@echo "AR $@"
	$(Q)$(AR) $(AFLAGS) -o $@ $^

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "clean"
	$(Q)rm -f $(lib) $(objs) $(deps)

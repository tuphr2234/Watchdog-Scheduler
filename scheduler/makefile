# Compiler and compiler flags
CC = gcc
SCHED_CFLAGS = -Wall -Wextra -fPIC -Ischeduler/include
# Directory containing the source files
SCHED_SRC_DIR = scheduler/src
# List of source files
SRC_FILES = $(wildcard $(SCHED_SRC_DIR)/*.c)
# Name of the shared object
SCHED_SO_NAME = scheduler_lib.so
# Default target
all: $(SCHED_SO_NAME)

# Rule to build the shared object
$(SCHED_SO_NAME): $(SRC_FILES)
	$(CC) $(SCHED_CFLAGS) -shared -o $(SCHED_SO_NAME) $(SRC_FILES)

# Clean rule
clean:
	rm -f $(SCHED_SO_NAME)

.PHONY: all clean

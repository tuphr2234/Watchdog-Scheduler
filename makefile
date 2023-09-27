# Compiler and compiler flags
CC =gcc
DEBUG =-g
RELEASE =-DNDEBUG -O3

SCHED_CFLAGS =-Wall -ansi -pedantic-errors -Wextra -fPIC -Ischeduler/include
WD_CFLAGS =-Wall -ansi -pedantic-errors -Wextra -fPIC -Iinclude -Ischeduler/include -L.  

# Directory containing the source files
SCHED_SRC_DIR =scheduler/src
WD_SRC_DIR =src
# List of source files
SCHED_SRC_FILES =$(wildcard $(SCHED_SRC_DIR)/*.c)
WD_SRC_FILES =$(filter-out $(WD_SRC_DIR)/watchdog.c, $(wildcard $(WD_SRC_DIR)/*.c)) # List of source files excluding watchdog.c
WATCHDOG_SRC =$(WD_SRC_DIR)/watchdog.c
#bin dir
BIN_DIR = bin
ifeq ($(MAKECMDGOALS),debug)
BIN_DIR := $(BIN_DIR)/debug
endif
ifeq ($(MAKECMDGOALS),release)
BIN_DIR := $(BIN_DIR)/release
endif

# Name of the shared objects
SCHED_SO_NAME =scheduler_lib.so
WD_SO_NAME =watchdog_lib.so
WATCHDOG_OUT =watchdog

# Default targets
all: $(BIN_DIR)/$(SCHED_SO_NAME) $(BIN_DIR)/$(WD_SO_NAME) $(BIN_DIR)/$(WATCHDOG_OUT)
	rm -f $(BIN_DIR)/$(SCHED_SO_NAME)
# Rule to build the shared object
$(BIN_DIR)/$(SCHED_SO_NAME):$(SCHED_SRC_FILES)
	$(CC) $(SCHED_CFLAGS) -shared -o $(BIN_DIR)/$(SCHED_SO_NAME) $(SCHED_SRC_FILES)

$(BIN_DIR)/$(WD_SO_NAME):$(WD_SRC_FILES)
	$(CC) $(WD_CFLAGS) -shared -o $(BIN_DIR)/$(WD_SO_NAME) $(WD_SRC_FILES) -l:$(BIN_DIR)/$(SCHED_SO_NAME)

# Rule to build the watchdog.out
$(BIN_DIR)/$(WATCHDOG_OUT):$(WATCHDOG_SRC)
	$(CC) $(WD_CFLAGS) -o $(BIN_DIR)/$(WATCHDOG_OUT) $(WATCHDOG_SRC) -l:$(BIN_DIR)/$(WD_SO_NAME)

# Debug and Release targets
debug: SCHED_CFLAGS +=$(DEBUG)
debug: WD_CFLAGS +=$(DEBUG)
debug: all

release: SCHED_CFLAGS +=$(RELEASE)
release: WD_CFLAGS +=$(RELEASE)
release: all

# Clean rule
clean:
	rm -f $(BIN_DIR)/$(SCHED_SO_NAME) $(BIN_DIR)/$(WD_SO_NAME) $(BIN_DIR)/$(WATCHDOG_OUT)

.PHONY: all clean debug release

########################################################################
# A generic Makefile for C++ programs
#
# Originally Created by Bob Li
# March 14, 2012
########################################################################
#
# Usage:
#
# Synopsis:
#
# make [DEBUG=0|1] [TEST=0|1]
#
# Options:
# Debug=0|1 turns on all debug information [default 0]
#
# Supported Platform: MINGW32, CYGWIN, Linux
#
########################################################################

# System information
SUPPORTED_PLATFORMS := Linux, CYGWIN, MINGW32
export PLATFORM := $(findstring $(shell uname -s | sed 's,_.*,,'),$(SUPPORTED_PLATFORMS))
ifeq ($(PLATFORM),"")
    $(error Unknown platform: $(shell uname -s))
endif
# the top work directory
TOP_WORK_DIR := $(shell pwd)
# target name. Usually, only TARGET_NAME need to be changed
TARGET_NAME := mna
# include path. Default: .
INCLUDE_DIRS := .
# target path. Default: .
TARGET_PATH := .
# C++ compiler
ifeq ($(PLATFORM),CYGWIN)
    export CC := g++
else
    export CC := g++
endif
# Archieve utility
export AR := ar
export ARFLAGS := rus
# Lexer
export FLEX := flex
export BISON := bison
# case-insensitive parser
export FLEXFLAGS := -i
export BISONFLAGS := --define --yacc

# debug flag. Note: comment out the line below for the release version
ifeq ($(DEBUG),1)
    CONFIG := debug
    export DEBUG_FLAG := -O0 -ggdb -DDEBUG
    export FLEXFLAGS += -d
else
    CONFIG := release
    export DEBUG_FLAG := -O3 -DNDEBUG
endif

# control testing code
ifeq ($(TEST),1)
    export DEBUG_FLAG += -DTEST
endif

# libraries
LDFLAGS := -lm

# all compiler flags
export CFLAGS := -Wall -Wno-unused-variable $(DEBUG_FLAG)

# The excutable has ".exe" suffix on the CYGWIN and MINGW32 platforms.
ifeq ($(PLATFORM),Linux)
    TARGET_SUFFIX :=
else
    TARGET_SUFFIX := .exe
endif
# construct the full target name
TARGET := $(TARGET_PATH)/$(TARGET_NAME)-$(PLATFORM)-$(CONFIG)$(TARGET_SUFFIX)

# the prerequisite of each source file will be generated automatically
# the suffix of source files
export SRCS_SUFFIX := .cpp
# the suffix of rerequisite files
export PREREQ_SUFFIX := .d

SRCS := $(wildcard *$(SRCS_SUFFIX))
OBJS := $(patsubst %$(SRCS_SUFFIX),%.o,$(wildcard *$(SRCS_SUFFIX)))
PREREQS := $(SRCS:%$(SRCS_SUFFIX)=%$(PREREQ_SUFFIX))

# subdirectory
PARSER_DIR := parser
export PARSER_LIB := parser.a
ELEMENT_DIR := element
export ELEMENT_LIB := element.a
MNA_DIR := mna
export MNA_LIB := mna.a
DB_DIR := db
export DB_LIB := db.a

# the wrapper script name
WRAPPER := mna.sh

.PHONY: all parser_lib element_lib mna_lib db_lib
all: $(TARGET)
	@echo "Generate the wrapper: $(WRAPPER)"; \
	$(RM) $(WRAPPER); \
	echo "#!/bin/bash" > $(WRAPPER); \
	echo "if [[ \$$1 == -* ]]; then" > $(WRAPPER); \
	echo "	$(TOP_WORK_DIR)/run.pl --binary $(TOP_WORK_DIR)/$(TARGET) \$$*" >> $(WRAPPER); \
	echo "else" >> $(WRAPPER); \
	echo "	$(TOP_WORK_DIR)/run.pl --binary $(TOP_WORK_DIR)/$(TARGET) -i \$$*" >> $(WRAPPER); \
	echo "fi" >> $(WRAPPER); \
	chmod +x $(WRAPPER)

ifneq "$(MAKECMDGOALS)" "clean"
ifneq "$(MAKECMDGOALS)" "distclean"
    sinclude $(PREREQS)
endif
endif

LIB_LIST := \
    $(PARSER_DIR)/$(PARSER_LIB) \
    $(ELEMENT_DIR)/$(ELEMENT_LIB) \
    $(MNA_DIR)/$(MNA_LIB) \
    $(DB_DIR)/$(DB_LIB)

$(TARGET): $(OBJS) $(LIB_LIST)
	@echo "Building $(TARGET) ..."
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "Successfully build $(TARGET) ."

$(PARSER_DIR)/$(PARSER_LIB): parser_lib
	$(MAKE) --directory=$(PARSER_DIR)

$(ELEMENT_DIR)/$(ELEMENT_LIB): element_lib
	$(MAKE) --directory=$(ELEMENT_DIR)

$(MNA_DIR)/$(MNA_LIB): mna_lib
	$(MAKE) --directory=$(MNA_DIR)

$(DB_DIR)/$(DB_LIB): db_lib
	$(MAKE) --directory=$(DB_DIR)

%$(PREREQ_SUFFIX): %$(SRCS_SUFFIX)
	$(CC) -MM $< $(CFLAGS) $(addprefix -I, $(INCLUDE_DIRS)) > $@.$$$$; \
	sed 's,\($*\)\.o[:]*,\1.o $@:,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

%.o: %$(SRCS_SUFFIX)
	$(CC) -c $< -o $@ $(CFLAGS) $(addprefix -I,$(INCLUDE_DIRS))

.PHONY: clean distclean

clean:
	$(MAKE) --directory=$(PARSER_DIR) distclean
	$(MAKE) --directory=$(ELEMENT_DIR) distclean
	$(MAKE) --directory=$(MNA_DIR) distclean
	$(MAKE) --directory=$(DB_DIR) distclean
	$(RM) $(OBJS) $(PREREQS)

distclean: clean
	$(RM) $(TARGET_PATH)/$(TARGET_NAME)-$(PLATFORM)-*$(TARGET_SUFFIX)


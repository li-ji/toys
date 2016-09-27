# include path. Default: .
INCLUDE_DIRS := ..
# the prerequisite of each source file will be generated automatically
# the suffix of source files
export SRCS_SUFFIX := .cpp
# the suffix of rerequisite files
export PREREQ_SUFFIX := .d

SRCS := $(wildcard *$(SRCS_SUFFIX))
OBJS := $(patsubst %$(SRCS_SUFFIX),%.o,$(wildcard *$(SRCS_SUFFIX)))
PREREQS := $(SRCS:%$(SRCS_SUFFIX)=%$(PREREQ_SUFFIX))

.PHONY: all clean distclean
all: $(MNA_LIB)

ifneq "$(MAKECMDGOALS)" "clean"
ifneq "$(MAKECMDGOALS)" "distclean"
    sinclude $(PREREQS)
endif
endif

$(MNA_LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%$(PREREQ_SUFFIX): %$(SRCS_SUFFIX)
	$(CC) -MM $< $(CFLAGS) $(addprefix -I, $(INCLUDE_DIRS)) > $@.$$$$; \
	sed 's,\($*\)\.o[:]*,\1.o $@:,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

%.o: %$(SRCS_SUFFIX)
	$(CC) -c $< -o $@ $(CFLAGS) $(addprefix -I,$(INCLUDE_DIRS))

.PHONY: clean distclean

clean:
	$(RM) $(OBJS) $(PREREQS)

distclean: clean
	$(RM) $(MNA_LIB)


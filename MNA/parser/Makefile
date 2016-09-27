
TEST_DIR := test
export TEST_OBJ := test.o

.PHONY: all test_lib clean distclean
all: $(PARSER_LIB)

ifeq ($(TEST),1)
$(PARSER_LIB): lex.yy.o y.tab.o parser.o $(TEST_DIR)/$(TEST_OBJ)
	$(AR) $(ARFLAGS) $@ $^
else
$(PARSER_LIB): lex.yy.o y.tab.o parser.o
	$(AR) $(ARFLAGS) $@ $^
endif

parser.o: parser.cpp ../parser.h
	$(CC) -I.. $(CFLAGS) -c $< -o $@

lex.yy.o: lex.yy.c
	$(CC) -I.. $(CFLAGS) -c $< -o $@

lex.yy.c: p_l.l y.tab.h ../parser.h
	$(FLEX) $(FLEXFLAGS) $<

y.tab.o: y.tab.c y.tab.h
	$(CC) -I.. $(CFLAGS) -c $< -o $@

y.tab.c y.tab.h: p_y.y ../element.h
	$(BISON) $(BISONFLAGS) $<

ifeq ($(TEST),1)
$(TEST_DIR)/$(TEST_OBJ): test_lib
	$(MAKE) --directory=$(TEST_DIR)
endif

clean:
	$(RM) lex.yy.c lex.yy.o parser.o y.tab.c y.tab.o y.tab.h
	$(MAKE) --directory=$(TEST_DIR) distclean

distclean: clean
	$(RM) $(PARSER_LIB)

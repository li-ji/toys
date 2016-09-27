#ifndef __P_H__
#define __P_H__
#include <cstddef> // size_t
//! reset the lexer status
void p_lex_init(void);

//! parse a string
size_t p_scan_string(const char * str);

//! delete the parser buffer
void p_delete_buffer(void);

//! Bison parser
int yyparse();

//! Parse an input netlist line by line.
//! Return 1 if the file cannot be opened; return 0 otherwise.
bool parse(const char *input_file_name);

#ifdef TEST
// Test the parser
void test_parser();
#endif // TEST
#endif // __P_H__

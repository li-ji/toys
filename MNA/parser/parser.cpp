#include "parser.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

//! Parse an input netlist line by line.
//! Return 1 if the file cannot be opened; return 0 otherwise.
bool parse(const char *input_file_name)
{
  assert(input_file_name);
  ifstream ifstrm(input_file_name);
  if (!ifstrm.good())
    {
      cerr << "Error: cannot open file: " << input_file_name << endl;
      return 1; // failed to open the input file
    }
  while (ifstrm.good())
    {
      string line;
      getline(ifstrm, line);
      p_lex_init();
      p_scan_string(line.c_str());
      yyparse(); // parse this line
      p_delete_buffer();
    }
  return 0;
}

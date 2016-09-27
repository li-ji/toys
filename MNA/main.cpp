#include "parser.h"
#include "mna.h"
#include "db.h"
#include <string>
#include <iostream>
#include <cstring>
#include <cassert>
using namespace std;

//! local variables
namespace
{
  const string DEFAULT_OUTPUT_PREFIX("mna");
  const string DEFAULT_OUTPUT_SUFFIX(".tex");
}
#ifdef TEST
//! the main test routine
void test()
{
  static const string TEST_PREFIX("TEST");
  cout << "####Test Begin####" << endl;
  test_parser();
  DB().stamp(MNA());
  MNA().print();
  MNA().print(TEST_PREFIX+DEFAULT_OUTPUT_SUFFIX.c_str());
  cout << " ####Test End####" << endl;
}
#endif //TEST

//! parse command-line options
//! Syntax:
//!   SIM input_netlist_name -o output_prefix
//! Return 1 if any syntax error; return 0 otherwise.
bool parse_cmdline_option(int argc, char **argv,
			  string &ifile_name, string &ofile_prefix)
{
  if (argc < 2)
    {
      cerr << "No netlist specified!" << endl;
      return 1;
    }
  // skip the program name, argv[0]
  ifile_name = string(argv[1]); // the 1st argument is the input file name
  // parse rest arguments
  // if no option is specified, the output will be dumped on the screen
  enum ArgType {FLAG, OUTPUT/*, FORMAT*/};
  ArgType argt = FLAG;
  for (int i=2; i<argc; ++i)
      switch (argt)
	{
	case FLAG: // -o, -fmt, etc.
	  if (!strcmp("-o", argv[i]))
	    argt = OUTPUT;
	  else
	    cerr << "Unknown flag: " << argv[i] << endl;
	break;
	case OUTPUT:
	  ofile_prefix = string(argv[i]);
	  argt = FLAG; // reset the argument type
	break;
	default:
	  assert(0);
	}
  if (ofile_prefix.empty())
    ofile_prefix = DEFAULT_OUTPUT_PREFIX; // use the default output file name prefix
  return 0;
}

//! The main routine
int main( int argc, char **argv)
{
#ifndef TEST
  string input_file, output_prefix;
  if (parse_cmdline_option(argc, argv, input_file, output_prefix))
    return 1; // fail to parse the command-line options
  if (parse(input_file.c_str()))
    return 1; // fail to parse the input netlist
  DB().stamp(MNA());
  MNA().print((output_prefix + DEFAULT_OUTPUT_SUFFIX).c_str());
#else
  test(); // run the test cases
#endif //TEST
  return 0;
}

#include "parser.h"

// Test the parser
void test_parser()
{
  static const char *stmt[] =
    {
      "R_1 n1 n2 1k",
      "C_1 n1 n2 2p",
      "L_1 n1 n2 3e-6h",
      "V_1 n1 n2 4.0v",
      "I_1 n1 n2 5.0mA",
      "E_1 n1 n2 in1 !gNd 1.1",
      "E_1 n1 n2 opAMp in1 !gNd 1.1",
      "F_1 n1 n2 V_1 1.2",
      "G_1 n1 n2 in1 ground 1.3",
      "H_1 n1 n2 V_1 1.4",
      "R_{edefined} 3 1 1k",
      "R_{edefined} 0 1 1k",
      "*comment1",
      "$comment2",
      " Unsupported line",
      0 // end of test string array
    };
  for (const char **p = stmt; *p; ++p)
    {
      p_lex_init();
      p_scan_string(*p);
      yyparse(); // parse 1 line
      p_delete_buffer();
    }
}

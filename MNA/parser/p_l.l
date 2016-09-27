%{
#include "parser.h"
#include "y.tab.h"
#include <list>
#include <string>
#include <memory>
#include <cstring>
#include <cassert>
using namespace std;

// declaration of the Lexer
#define YY_DECL int yylex(YYSTYPE *lvalp)
YY_DECL;

    /** This is added to avoid a warning from the lex-generated code */
#define YY_NO_UNPUT 1

    /** this is here to remove make warning:
     * yy_flex_realloc defined but not used
     */
#ifdef YY_USES_REJECT
#undef YY_USES_REJECT
#endif /* YY_USES_REJECT */

/* global Lexer variables */
static int _num_nodes = 0; /* number of nodes that have already been parsed */
static int _expected_num_nodes = 0; /* expected number of nodes */
static bool _has_current_ctrl = false; /* CCCS and CCVS has a current controlling branch */
static list<char *> _tokbuffer; /* hold copies of tokens */

/* pass current token string to the Bison parser */
static void _save_string(YYSTYPE *lvalp, const char *yytext, size_t yyleng);

/* pass a node name token to YACC */
static void _save_node_name(YYSTYPE *lvalp, const char *yytext, size_t yyleng);
%}

ws      [ \t]+
line_continuation1 [ \t]+[\\]$
  /* ' ', '\t', '\n', '\v', '\f', '\r'*/
line_continuation2 [[:space:]]+\+

  /* HSPICE node name patterns */
gnd_node_name !GND|GND|GROUND|0
node_name_head  [[:alnum:]~!@#%^&_?|]
node_name_body ([[:alnum:]~!@#%^&_?|;:\-+<>{}\[\]])|(\\\\)
node_name {node_name_head}{node_name_body}*

 /* supported elements
    Resistor:
	Rxxx n+ n- ...
    Capacitor:
	Cxxx n+ n- ...
    Inductor:
	Lxxx n+ n- ...
    Independent DC voltage source:
	Vxxx n+ n- ...
    Independent DC current source:
	Ixxx n+ n- ...
    Voltage-controlled voltage source (VCVS), or E-element:
	Exxx n+ n- in+ in-
    Current-controlled current source (CCCS), or F-element:
	Fxxx n+ n- vn1
    Voltage-controlled current source (VCCS), or G-element:
	Gxxx n+ n- in+ in-
    Current-controlled voltage source (CCVS), or H-element:
	Hxxx n+ n- vn1
  */

  /* HSPICE instance name patterns */
inst_name_body  [[:alnum:]~!@#$%^&*_?|;\-+?/{}\[\]]*

%x NODE
%x CC

%x SKIP

 /*%option debug*/
%option noyywrap

%%

<INITIAL,NODE>{ws}    /* skip spaces */
<INITIAL,NODE>\n /* skip single \n */

<INITIAL>[*$].* /* skip one-line comments */

<INITIAL>[^RCLVIEFGH*$].* /* skip unsupported lines */

<INITIAL>[RCLVIEFGH]{inst_name_body} {
    BEGIN(NODE);
    /* node counters must have been set before */
    assert(!_num_nodes && !_expected_num_nodes && !_has_current_ctrl);
    _expected_num_nodes = 2;
    _save_string(lvalp, yytext, yyleng);
    switch (yytext[0])
      {
      case 'R': case 'r': return tokR;
      case 'C': case 'c': return tokC;
      case 'L': case 'l': return tokL;
      case 'V': case 'v': return tokV;
      case 'I': case 'i': return tokI;
      case 'E': case 'e': _expected_num_nodes += 2; return tokVCVS;
      case 'F': case 'f': _has_current_ctrl = true; return tokCCCS;
      case 'G': case 'g': _expected_num_nodes += 2; return tokVCCS;
      case 'H': case 'h': _has_current_ctrl = true; return tokCCVS;
      default: assert(0); /* never gets here */
      }
}

<NODE>{line_continuation1} /* skip line continuation */
<NODE>{line_continuation2} /* skip line continuation */
<NODE>OPAMP { /* only VCVS has this token */
    /* this must be the 3rd token in the node list */
    assert(_expected_num_nodes==4 && _num_nodes==2);
    return tokOPAMP;
}
<NODE>{gnd_node_name} {
    /* translate all patterns of GND nodes to the unique form, "0" */
    _save_node_name(lvalp, "0", 1);
    return tokNODE;
}
<NODE>{node_name} {
    /* process non-GND node names */
    _save_node_name(lvalp, yytext, yyleng);
    return tokNODE;
}

<CC>V{inst_name_body} {
    _has_current_ctrl = false; /* reset the flag */
    /* process current controlling branch */
    _save_string(lvalp, yytext, yyleng);
    BEGIN(SKIP);
    return tokCC;
}

<SKIP>.* { /* skip rest of a line */
    assert(_num_nodes == _expected_num_nodes);
    BEGIN(INITIAL);
}

%%

// pass current token string to the Bison parser
static void _save_string(YYSTYPE *lvalp, const char *yytext, size_t yyleng)
{
  _tokbuffer.push_back(strcpy(new char[yyleng+1], yytext));
  lvalp->String = _tokbuffer.back();
}

/* pass a node name token to YACC */
static void _save_node_name(YYSTYPE *lvalp, const char *yytext, size_t yyleng)
{
  assert(_num_nodes < _expected_num_nodes);
  _save_string(lvalp, yytext, yyleng);
  if (++_num_nodes == _expected_num_nodes)
    {
      if (!_has_current_ctrl)
	{
	  _num_nodes = _expected_num_nodes = 0; /* reset node counters */
	  BEGIN(SKIP); /* skip rest of current line */
	}
      else /* CCCS or CCVS */
	{
	  BEGIN(CC);
	}
    }
}

// reset the lexer status
void p_lex_init(void)
{
  BEGIN(INITIAL);
  _num_nodes = _expected_num_nodes = 0;
  _has_current_ctrl = false;
}

// parse a string
size_t p_scan_string(const char * str)
{
  assert(str);
  yy_switch_to_buffer(yy_scan_string(str));
  return strlen(str);
}
// delete the parser buffer
void p_delete_buffer(void)
{
  yy_delete_buffer(YY_CURRENT_BUFFER);
  for (list<char *>::iterator
       it = _tokbuffer.begin(); it != _tokbuffer.end(); ++it)
    delete[] *it;
  _tokbuffer.clear();
  // reset global parser flags
  _num_nodes = _expected_num_nodes = 0;
  _has_current_ctrl = false;
}

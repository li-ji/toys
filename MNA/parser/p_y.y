%{
#include "element.h"
#include "db.h"
#include <iostream>
#include <map>
#include <cassert>

//#define YYDEBUG 1

// following block is copied from y.tab.h
#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef union YYSTYPE {
    const char *String;
} YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

// declaration of the Lexer
#define YY_DECL int yylex(YYSTYPE *lvalp)
YY_DECL;

// yyerror() required by the parser
void yyerror(const char *msg);

// unique global interface to access the MNA matrix
MNAType &MNA();

%}

%union
{
    const char *String;
}

%token <String> tokR tokC tokL tokV tokI
%token <String> tokVCVS tokCCCS tokVCCS tokCCVS tokOPAMP
%token <String> tokNODE tokCC

%pure-parser
%error-verbose

%%
statement: /* empty: comments or unsupported lines */
| two_port_inst_stmt
| four_port_inst_stmt
;
two_port_inst_stmt:
  tokR tokNODE tokNODE {
    Resistor *tmp = new Resistor;
    tmp->setName($1);
    tmp->appendNode($2); tmp->appendNode($3);
    tmp->print();
    DB().appendElement($1, tmp);
}
| tokC tokNODE tokNODE {
    Capacitor *tmp = new Capacitor;
    tmp->setName($1);
    tmp->appendNode($2); tmp->appendNode($3);
    tmp->print();
    DB().appendElement($1, tmp);
}
| tokL tokNODE tokNODE {
    Inductor *tmp = new Inductor;
    tmp->setName($1);
    tmp->appendNode($2); tmp->appendNode($3);
    tmp->print();
    DB().appendElement($1, tmp);
}
| tokV tokNODE tokNODE {
    VSource *tmp = new VSource;
    tmp->setName($1);
    tmp->appendNode($2); tmp->appendNode($3);
    tmp->print();
    DB().appendElement($1, tmp);
}
| tokI tokNODE tokNODE {
    ISource *tmp = new ISource;
    tmp->setName($1);
    tmp->appendNode($2); tmp->appendNode($3);
    tmp->print();
    DB().appendElement($1, tmp);
}
| tokCCCS tokNODE tokNODE tokCC {
    CCCS *tmp = new CCCS;
    tmp->setName($1);
    tmp->appendNode($2); tmp->appendNode($3);
    tmp->setCC($4);
    tmp->print();
    DB().appendElement($1, tmp);
}
| tokCCVS tokNODE tokNODE tokCC {
    CCVS *tmp = new CCVS;
    tmp->setName($1);
    tmp->appendNode($2); tmp->appendNode($3);
    tmp->setCC($4);
    tmp->print();
    DB().appendElement($1, tmp);
}
;
four_port_inst_stmt:
  tokVCVS tokNODE tokNODE tokNODE tokNODE {
    VCVS *tmp = new VCVS;
    tmp->setName($1);
    tmp->appendNode($2); tmp->appendNode($3);
    tmp->appendNode($4); tmp->appendNode($5);
    tmp->print();
    DB().appendElement($1, tmp);
}
| tokVCCS tokNODE tokNODE tokNODE tokNODE {
    VCCS *tmp = new VCCS;
    tmp->setName($1);
    tmp->appendNode($2); tmp->appendNode($3);
    tmp->appendNode($4); tmp->appendNode($5);
    tmp->print();
    DB().appendElement($1, tmp);
}
| tokVCVS tokNODE tokNODE tokOPAMP tokNODE tokNODE {
    OPAMP *tmp = new OPAMP;
    tmp->setName($1);
    tmp->appendNode($2); tmp->appendNode($3);
    tmp->appendNode($5); tmp->appendNode($6);
    tmp->print();
    DB().appendElement($1, tmp);
}
;
%%

// yyerror() required by the parser
void yyerror(const char *msg)
{
  std::cerr << msg << std::endl;
}

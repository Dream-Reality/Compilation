#include "../eeyore2tigger/parser/parser.tab.hpp"
#include <stdio.h>
#include <string.h>
#include "../eeyore2tigger/ast/node.h"
#include "../eeyore2tigger/eeyore2tigger.h"
extern int eeyore2tigger_lex(void);
extern int eeyore2tigger_parse(void);
extern void eeyore2tigger__scan_string(const char* s);
extern ProgramsNode* root;
extern std::stringstream eeyore_code;
std::stringstream tigger_code;
void eeyore2tigger() {
  // freopen(argv[3],"r",stdin);
  // freopen(argv[5],"w",stdout);
  eeyore2tigger__scan_string(eeyore_code.str().c_str()); 
  eeyore2tigger_parse();
  // root->printInfo();
  root->analyze();
  //printf("%s\n",tigger_code.str().c_str());
}

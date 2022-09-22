#include "parser/parser.tab.hpp"
#include <stdio.h>
//#include "ast/node.h"
#include "ir/ir.h"
#include "sysy2eeyore.h"
//extern int yylex(void);
extern int sysy2eeyore_parse(void);
extern CompUnitsNode* sysy2eeyore_root;
//extern int ir::label_count;
std::stringstream eeyore_code;
void sysy2eeyore() {

  // freopen(argv[3],"r",stdin);
  // freopen(argv[5],"w",stdout);
  sysy2eeyore_parse();
  
  // sysy2eeyore_root->printInfo();
  ir::generat_ir(sysy2eeyore_root);
  // cout<<eeyore_code.str()<<endl;
  // return 0;
  // printf("%d\n",f);
  // return f;
}

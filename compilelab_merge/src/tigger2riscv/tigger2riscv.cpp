#include<stdio.h>
#include"parser/parser.tab.hpp"
#include"tigger2riscv.h"
#include<sstream>
extern int tigger2riscv_parse(void);
extern std::stringstream tigger_code;
extern void tigger2riscv__scan_string(const char* s);
void tigger2riscv(){

    // freopen(argv[2],"r",stdin);
    // freopen(argv[4],"w",stdout);
    tigger2riscv__scan_string(tigger_code.str().c_str()); 
    tigger2riscv_parse();
}


const char* Reserved_Word_String[][2] = {{"if","IF"},
                                        {"else","ELSE"},
                                        {"while","WHILE"},
                                        {"for","FOR"},
                                        {"break","BREAK"},
                                        {"continue","CONTINUE"},
                                        {"return","RETURN"},
                                        {"const","CONST"},
                                        {"int","INT"},
                                        {"void","VOID"}};
const char* Operation_String[][2] = {{"=","ASSIGN"},
                                     {"==","EQ"},
                                     {"!=","NE"},
                                     {"<","LT"},
                                     {"<=","LE"},
                                     {">","GT"},
                                     {">=","GE"},
                                     {"&","AND"},
                                     {"|","OR"},
                                     {"++","DOUBLEPLUS"},
                                     {"--","DOUBLEMINUS"},
                                     {"+","PLUS"},
                                     {"-","MINUS"},
                                     {"*","MUL"},
                                     {"/","DIV"},
                                     {"%","MOD"},
                                     {"!","NOT"},
                                     {".","DOT"}};
const char* Symbol_String[][2] = {{"(","LPAREN"},
                                  {")","RPAREN"},
                                  {"[","LSQUARE"},
                                  {"]","RSQUARE"},
                                  {"{","LBRACE"},
                                  {"}","RBRACE"},
                                  {",","COMMA"},
                                  {";","SEMI"}};

int Start_Reserved_Word = 0;
int Start_Operation = 100;
int Start_Symbol = 200;
const char* Number2Symbol(int number){
    if (number>=Start_Symbol)return Symbol_String[number-Start_Symbol][0];
    else if (number>=Start_Operation)return Operation_String[number-Start_Operation][0];
    else if (number>=Start_Reserved_Word)return Reserved_Word_String[number-Start_Reserved_Word][0];
    else {
        printf("wrong number!");return NULL;
    }
}

void Print_Define(){
    int Count_Reserved_Word = sizeof(Reserved_Word_String)/16;
    int Count_Operation = sizeof(Operation_String)/16;
    int Count_Symbol = sizeof(Symbol_String)/16;
    for (int i = 0; i<Count_Reserved_Word;i++)
        printf("#define %s %d\n",Reserved_Word_String[i][1],i+Start_Reserved_Word);
    for (int i = 0; i<Count_Operation;i++)
        printf("#define %s %d\n",Operation_String[i][1],i+Start_Operation);
    for (int i = 0; i<Count_Symbol;i++)
        printf("#define %s %d\n",Symbol_String[i][1],i+Start_Symbol);


    for (int i = 0; i<Count_Reserved_Word;i++)
        printf("\"%s\"\t\t\t\t\t\t\t\tReserved_Word_Handler(%s);\n",Reserved_Word_String[i][0],Reserved_Word_String[i][1]);
    for (int i = 0; i<Count_Operation;i++)
        printf("\"%s\"\t\t\t\t\t\t\t\tOperation_Handler(%s);\n",Operation_String[i][0],Operation_String[i][1]);
    for (int i = 0; i<Count_Symbol;i++)
        printf("\"%s\"\t\t\t\t\t\t\t\tSymbol_Handler(%s);\n",Symbol_String[i][0],Symbol_String[i][1]);
}
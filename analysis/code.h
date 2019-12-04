#ifndef CODE_H_INCLUDED
#define CODE_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>
#include "list.h"
#include <map>

map<string, int> opMap{{"=", 0}, {"+", 1}, {"-", 2}, {"*", 3}, {"/", 4}};

class CodeGenerate
{
    public:
    CodeGenerate()
    {

    }
    ~CodeGenerate()
    {

    }
    void generateCode(List &parserList)
    {
        ofstream code("\\assembly\\out.s");
        ThreeAddress *p = parserList.head;
        while(p != 0)
        {
            switch(opMap[p->op])
            {
                case: opMap["="]: code << "MOV";
                case: opMap["+"]: code << "ADD";
            }
            p = p->next;
        }
    }
};

#endif // CODE_H_INCLUDED
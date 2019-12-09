#ifndef CODE_H_INCLUDED
#define CODE_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>
#include "list.h"
#include <map>
#include <set>

map<string, int> keyMap{{"=", 0}, {"+", 1}, {"-", 2}, {"*", 3}, {"/", 4}, {"int", 5}, {"char", 6}, {"double", 7}, {"label", 8}, {"cmp", 9}, 
{"J", 10}};
set<string> cmpMap{">", ">=", "<=", "<", "=="};

class CodeGenerate
{
    ofstream code;
    public:
    CodeGenerate()
    {
        code.open("out.asm");
    }
    ~CodeGenerate()
    {

    }
    void generateData(map<string, IdValue *> *idMap)
    {
        code << endl << endl << "_DATA SEGMENT" << endl;
        map<string, IdValue *>::iterator iter;
        for(iter = (*idMap).begin(); iter != (*idMap).end(); iter++)
        {
            string type = "DD";
            switch(keyMap[iter->second->type])
            {
                // int
                case 5: type = "DD"; break;
                // char
                case 6: type = "DD"; break;
                // double
                case 7: type = "DQ"; break;
            }
            code << iter->first << "\t" << type << "\t0" << endl;
        }
        code << "_DATA END" << endl;
    }
    void generateCode(List &parserList)
    {
        code << endl << endl << "_TEXT SEGMENT" <<endl;
        ThreeAddress *p = parserList.head;
        while(p != 0)
        {
            switch(keyMap[p->op])
            {
                // =
                case 0: 
                {
                    code << "mov DWORD PTR " << p->result << ", DWORD PTR " << p->arg1 << endl;
                    break;
                }
                // +
                case 1: 
                {
                    code << "mov eax, DWORD PTR " << p->arg1 << endl;
                    code << "add eax, DWORD PTR " << p->arg2 << endl;
                    code << "mov DWORD PTR " << p->result << ", eax" << endl;
                    break;
                }
                // -
                case 2: 
                {
                    code << "mov eax, DWORD PTR " << p->arg1 << endl;
                    code << "sub eax, DWORD PTR " << p->arg2 << endl;
                    code << "mov DWORD PTR " << p->result << ", eax" << endl;
                    break;
                }
                // *
                case 3: 
                {
                    code << "mov eax, DWORD PTR " << p->arg1 << endl;
                    code << "mul eax, DWORD PTR " << p->arg2 << endl;
                    code << "mov DWORD PTR " << p->result << ", eax" << endl;
                    break;
                }
                // /
                case 4: 
                {
                    code << "mov eax, DWORD PTR " << p->arg1 << endl;
                    code << "cdq" << p->arg1 << endl;
                    code << "idiv DWORD PTR " << p->arg2 << endl;
                    code << "mov DWORD PTR " << p->result << ", eax" << endl;
                    break;
                }
            }
            p = p->next;
        }
        code << "_TEXT END" <<endl;
    }
    void generate(List &parserList)
    {
        map<string, IdValue *> *idMap = parserList.id_map_stack->top();
        generateData(idMap);
        generateCode(parserList);
    }
};

#endif // CODE_H_INCLUDED
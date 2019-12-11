#ifndef CODE_H_INCLUDED
#define CODE_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>
#include "list.h"
#include <map>
#include <set>

map<string, int> keyMap{{"+", 1}, {"-", 2}, {"*", 3}, {"/", 4}, {"=", 5}, {"int", 6}, {"char", 7}, {"double", 8}, {"label", 9}, {"cmp", 10}, 
{"j", 11}, {"je", 11}, {"jne", 11}, {"jl", 11}, {"jle", 11}, {"jg", 11}, {"jge", 11}};

string commonHeader = ".386\n"
".model flat, stdcall\n"
"option casemap:none\n"
"include \\masm32\\include\\windows.inc\n"
"include \\masm32\\include\\user32.inc\n"
"include \\masm32\\include\\kernel32.inc\n"
"includelib \\masm32\\lib\user32.lib\n"
"includelib \\masm32\\lib\\kernel32.lib\n";

class CodeGenerate
{
    ofstream code;
    string asm_path;
    public:
    CodeGenerate(string c_path)
    {
        asm_path = "out.asm";
        for(int i = c_path.length(); i >= 0; i--)
        {
            if(c_path[i] == '.')
            {
                asm_path = c_path.substr(0, i);
                asm_path += ".asm";
                break;
            }
        }
    }
    ~CodeGenerate()
    {

    }
    void generateData(map<string, IdValue *> *idMap)
    {
        code << "\n\n.data\n";
        map<string, IdValue *>::iterator iter;
        for(iter = (*idMap).begin(); iter != (*idMap).end(); iter++)
        {
            string type = "DD";
            switch(keyMap[iter->second->type])
            {
                // int
                case 6: type = "DD"; break;
                // char
                case 7: type = "DD"; break;
                // double
                case 8: type = "DQ"; break;
            }
            code << iter->first << "\t" << type << "\t0\n";
        }
    }
    void generateCode(List &parserList)
    {
        code << "\n\n" << ".code" <<"\n";
        ThreeAddress *p = parserList.head;
        while(p != 0)
        {
            switch(keyMap[p->op])
            {
                // +
                case 1: 
                {
                    code << "mov eax, DWORD PTR " << p->arg1 << "\n";
                    code << "add eax, DWORD PTR " << p->arg2 << "\n";
                    code << "mov DWORD PTR " << p->result << ", eax" << "\n";
                    break;
                }
                // -
                case 2: 
                {
                    code << "mov eax, DWORD PTR " << p->arg1 << "\n";
                    code << "sub eax, DWORD PTR " << p->arg2 << "\n";
                    code << "mov DWORD PTR " << p->result << ", eax" << "\n";
                    break;
                }
                // *
                case 3: 
                {
                    code << "mov eax, DWORD PTR " << p->arg1 << "\n";
                    code << "mul eax, DWORD PTR " << p->arg2 << "\n";
                    code << "mov DWORD PTR " << p->result << ", eax" << "\n";
                    break;
                }
                // /
                case 4: 
                {
                    code << "mov eax, DWORD PTR " << p->arg1 << "\n";
                    code << "cdq" << p->arg1 << "\n";
                    code << "idiv DWORD PTR " << p->arg2 << "\n";
                    code << "mov DWORD PTR " << p->result << ", eax" << "\n";
                    break;
                }
                // =
                case 5: 
                {
                    code << "mov DWORD PTR " << p->result << ", DWORD PTR " << p->arg1 << "\n";
                    break;
                }
                // label
                case 9: 
                {
                    code << "$" << p->result << "\n";
                    break;
                }
                // cmp
                case 10: 
                {
                    code << "cmp DWORD PTR " << p->result << ", DWORD PTR " << p->arg1 << "\n";
                    break;
                }
                // j_type
                case 11: 
                {
                    code << p->op << "short $" << p->result << "\n";
                    break;
                }
            }
            p = p->next;
        }
    }
    void generate(List &parserList)
    {
        code.open(asm_path);
        map<string, IdValue *> *idMap = parserList.id_map_stack->top();
        code << commonHeader;
        generateData(idMap);
        generateCode(parserList);
        code.close();
    }
};

#endif // CODE_H_INCLUDED
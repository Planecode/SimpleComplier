#ifndef CODE_H_INCLUDED
#define CODE_H_INCLUDED
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "list.h"
#include "table.h"
#include <map>
#include <set>

map<string, int> keyMap{{"+", 1}, {"-", 2}, {"*", 3}, {"/", 4}, {"=", 5}, {"int", 6}, {"char", 7}, {"double", 8}, {"label", 9}, {"cmp", 10}, 
{"jmp", 11}, {"je", 11}, {"jne", 11}, {"jl", 11}, {"jle", 11}, {"jg", 11}, {"jge", 11}, {"inc", 12}, {"para", 13}, {"INVOKE", 14},{"[]=", 15},
{"p=", 16}, {"=&", 17}, {"index", 18} };

string commonHeader = ".386\n"
".model flat, stdcall\n"
"option casemap:none\n"
"include \\masm32\\include\\windows.inc\n"
"include \\masm32\\include\\user32.inc\n"
"include \\masm32\\include\\kernel32.inc\n"
"includelib \\masm32\\lib\\user32.lib\n"
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
            if (iter->second->is_array)
            {
                cout << iter->first << "    " << type << "    " << iter->second->width << "    " << "DUP  (?)\n";
            }
            else
            {
                code << iter->first << "    " << type << "    0\n";
            }
            
        }
    }

    void generate_local_var(map<string, IdValue *> *id_map)
    {
        if(!id_map->empty())
        {
            bool first = 1;
            code << "    LOCAL";
            map<string, IdValue *>::iterator iter;
            for(iter = id_map->begin(); iter != id_map->end(); iter++) 
            {
                if(first)
                {
                    first = 0;
                    code << " ";
                }
                else
                {
                    code << ",\n          ";
                }
                code << iter->first << ": DWORD";
            }
            code << "\n\n";
        }
    }
    void generate_func(string name, SegmentBlock *segment_block)
    {
        ThreeAddress *p = segment_block->begin_address;
        code << p->result << ":\n";
        code << "$" << name << " PROC";
        if(!segment_block->para_map->empty())
        {
            bool first = 1;
            code << ",\n";
            map<string, IdValue *>::iterator iter;
            for(iter = segment_block->para_map->begin(); iter != segment_block->para_map->end(); iter++) 
            {
                code << "    " << iter->first << ": DWORD\n";
            }
        }
        code << "\n";
        generate_local_var(segment_block->id_map);

        list<string> para_list;
        
        while(p != segment_block->end_address->next)
        {
            switch(keyMap[p->op])
            {
                // +
                case 1: 
                {
                    code << "    mov eax, " << get_instruction(p->arg1, segment_block, segment_block->id_map) << "\n";
                    code << "    add eax, " << get_instruction(p->arg2, segment_block, segment_block->id_map) << "\n";
                    code << "    mov " << p->result << ", eax" << "\n";
                    break;
                }
                // -
                case 2: 
                {
                    code << "    mov eax, " << get_instruction(p->arg1, segment_block, segment_block->id_map) << "\n";
                    code << "    sub eax, " << get_instruction(p->arg2, segment_block, segment_block->id_map) << "\n";
                    code << "    mov " << p->result << ", eax" << "\n";
                    break;
                }
                // *
                case 3: 
                {
                    code << "    mov eax, " << get_instruction(p->arg1, segment_block, segment_block->id_map) << "\n";
                    code << "    imul eax, " << get_instruction(p->arg2, segment_block, segment_block->id_map) << "\n";
                    code << "    mov " << p->result << ", eax" << "\n";
                    break;
                }
                // /
                case 4: 
                {
                    code << "    mov eax, " << get_instruction(p->arg1, segment_block, segment_block->id_map) << "\n";
                    code << "    idiv " << get_instruction(p->arg2, segment_block, segment_block->id_map) << "\n";
                    code << "    mov " << p->result << ", eax" << "\n";
                    break;
                }
                // =
                case 5: 
                {
                    code << "    mov eax, " << get_instruction(p->arg1, segment_block, segment_block->id_map) << "\n";
                    code << "    mov " << get_instruction(p->result, segment_block, segment_block->id_map) << ", eax" << "\n";
                    break;
                }
                // label
                case 9: 
                {
                    code << p->result << ":\n";
                    break;
                }
                // cmp
                case 10: 
                {
                    code << "    mov eax, " << get_instruction(p->arg1, segment_block, segment_block->id_map) << "\n";
                    code << "    cmp " << "eax, " << get_instruction(p->arg2, segment_block, segment_block->id_map) << "\n";
                    break;
                }
                // j_type
                case 11: 
                {
                    code << "    " << p->op << " " << get_instruction(p->result, segment_block, segment_block->id_map) << "\n";
                    break;
                }
                // inc
                case 12: 
                {
                    code << "    mov eax, " << p->arg1 << "\n";
                    code << "    inc eax\n";
                    code << "    mov " << p->arg1 << ", eax" << "\n";
                    break;
                }
                // para
                case 13: 
                {
                    para_list.push_back(p->result);
                    break;
                }
                // INVOKE
                case 14: 
                {
                    code << "    INVOKE $" << p->result;
                    while(!para_list.empty()) 
                    {
                        code << ", " << para_list.front();
                        para_list.pop_front();
                    }
                    code << "\n";
                    break;
                }
                // []=
                case 15:
                {
                    code << "    mov eax, " << get_instruction(p->arg1, segment_block, segment_block->id_map) << "\n";
                    code << "    mov esi, " <<  get_instruction(p->arg2, segment_block, segment_block->id_map) << "\n";
                    code << "    mov " << p->result << "[esi], eax\n";
                    break;
                }
                // p=
                case 16:
                {
                    code << "    mov esi, "<< p->arg1 << "\n";
                    code << "    mov "<< p->result << ", esi"<< "\n";
                    break;

                }
                // =&
                case 17:
                {
                    code << "    lea esi, " << "[" << p->arg1 << "]\n";
                    code << "    mov " << p->result << ", esi\n";
                    break;
                }
                // index
                case 18:
                {
                    code << "    mov eax, " << p->arg1 << "[" << p->arg2 << "]\n";
                    code << "    mov "<< p->result << ", eax\n";
                }
            }
            p = p->next;
        }
        code << name << "_end:\n";
        code << "    ret\n";
        code << "$" << name << " ENDP\n";
    }
    string get_instruction (string true_id, SegmentBlock *segment_block, map<string, IdValue *> *idMap)
    {
        if (is_num(true_id))
        {
            return true_id;
        }
        if ((*idMap)[true_id]->is_pointer)
            return "["+ true_id +"]";
        return true_id;
    }
    bool is_num (string id)
    {
        double a;
        stringstream in(id);
        if (in >> a)
            return true;
        else
            return false;
    }
    void generate(List &parserList)
    {
        code.open(asm_path);
        code << commonHeader;
        code << "\n\n" << ".code" <<"\n";
        list<string>::iterator iter;
        for(iter = IdList.begin(); iter !=IdList.end(); iter++) 
        {
            SegmentBlock *segment_block = IdTable[*iter];
            ThreeAddress *p = segment_block->begin_address;
            if(p->op == "entry")
            {
                generate_func(*iter, segment_block);
            }
        }
        code << "END $main\n";
        code.close();
    }
};

#endif // CODE_H_INCLUDED
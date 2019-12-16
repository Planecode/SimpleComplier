#ifndef CODE_H_INCLUDED
#define CODE_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>
#include "list.h"
#include "table.h"
#include <map>
#include <set>

map<string, int> keyMap{{"+", 1}, {"-", 2}, {"*", 3}, {"/", 4}, {"=", 5}, {"int", 6}, {"char", 7}, {"double", 8}, {"label", 9}, {"cmp", 10}, 
{"jmp", 11}, {"je", 11}, {"jne", 11}, {"jl", 11}, {"jle", 11}, {"jg", 11}, {"jge", 11}, {"inc", 12}, {"para", 13}, {"INVOKE", 14}, {"return", 15}, 
{"addr", 16}, {"[]=", 17}, {"index", 18}, {"=[]", 19}};

string commonHeader = ".386\n"
".model flat, stdcall\n"
"option casemap:none\n"
"include \\masm32\\include\\windows.inc\n"
"include \\masm32\\include\\user32.inc\n"
"include \\masm32\\include\\kernel32.inc\n"
"include \\masm32\\include\\masm32.inc\n"
"include \\masm32\\include\\msvcrt.inc\n"
"includelib \\masm32\\lib\\user32.lib\n"
"includelib \\masm32\\lib\\kernel32.lib\n"
"includelib \\masm32\\lib\\masm32.lib\n"
"includelib \\masm32\\lib\\msvcrt.lib\n"
".const\n"
"$output_format_int byte \"%d\", 0DH, 0AH, 0\n"
"$input_format_int byte \"%d\", 0\n"
".code\n"
"$print PROC,\n"
"    val: dword\n"
"    invoke crt_printf, addr $output_format_int, val\n"
"    ret\n"
"$print ENDP\n"
"$input PROC,\n"
"    val: ptr dword\n"
"    invoke crt_scanf, addr $input_format_int,  val\n"
"    ret\n"
"$input ENDP\n"
"$point PROC,\n"
"    val: ptr dword\n"
"    mov eax, val\n"
"    ret\n"
"$point ENDP\n";

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
            code << iter->first << "    " << type << "    0\n";
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
                IdValue *id_value  = iter->second;
                string type = "";
                switch(keyMap[id_value->type])
                {
                    // int
                    case 6: type = "DWORD"; break;
                    // char
                    case 7: type = "DWORD"; break;
                    // double
                    case 8: type = "SDWORD"; break;
                }
                if(id_value->is_array)
                {
                    code << iter->first << "[" << id_value->width << "]: " << type;
                }
                else if(id_value->is_pointer)
                {
                    code << iter->first << ": DWORD";
                }
                else
                {
                    code << iter->first << ": " << type;
                }
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
            map<string, IdValue *>::iterator iter;
            for(iter = segment_block->para_map->begin(); iter != segment_block->para_map->end(); iter++) 
            {
                code << ",\n    " << iter->first << ": DWORD";
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
                    code << "    mov eax, " << p->arg1 << "\n";
                    code << "    add eax, " << p->arg2 << "\n";
                    code << "    mov " << p->result << ", eax" << "\n";
                    break;
                }
                // -
                case 2: 
                {
                    code << "    mov eax, " << p->arg1 << "\n";
                    code << "    sub eax, " << p->arg2 << "\n";
                    code << "    mov " << p->result << ", eax" << "\n";
                    break;
                }
                // *
                case 3: 
                {
                    code << "    mov eax, " << p->arg1 << "\n";
                    code << "    imul eax, " << p->arg2 << "\n";
                    code << "    mov " << p->result << ", eax" << "\n";
                    break;
                }
                // /
                case 4: 
                {
                    code << "    mov eax, " << p->arg1 << "\n";
                    code << "    idiv " << p->arg2 << "\n";
                    code << "    mov " << p->result << ", eax" << "\n";
                    break;
                }
                // =
                case 5: 
                {
                    code << "    mov eax, " << p->arg1 << "\n";
                    code << "    mov " << p->result << ", eax" << "\n";
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
                    code << "    mov eax, " << p->arg1 << "\n";
                    code << "    cmp " << "eax, " << p->arg2 << "\n";
                    break;
                }
                // j_type
                case 11: 
                {
                    code << "    " << p->op << " " << p->result << "\n";
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
                // return
                case 15: 
                {
                    code << "    mov eax, " << p->result << "\n";
                    break;
                }
                // addr
                case 16: 
                {
                    code << "    INVOKE $point, addr " << p->arg1 << "\n";
                    code << "    mov " << p->result << ", eax" << "\n";
                    break;
                }
                // []=
                case 17: 
                {
                    code << "    mov eax, " << p->arg1 << "\n";
                    code << "    mov " << p->result << "[" + p->arg2 + "], eax" << "\n";
                    break;
                }
                // index
                case 18: 
                {
                    code << "    mov eax, " << p->arg1 << "[" + p->arg2 + "]\n";
                    code << "    mov " << p->result << ", eax" << "\n";
                    break;
                }
                // =[]
                case 19: 
                {
                    code << "    INVOKE $point, addr " << p->arg1 << "[0]\n";
                    code << "    mov " << p->result << ", eax" << "\n";
                    break;
                }
            }
            p = p->next;
        }
        code << name << "_end:\n";
        code << "    ret\n";
        code << "$" << name << " ENDP\n";
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
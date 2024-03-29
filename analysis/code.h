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
{"addr", 16}, {"[]=", 17}, {"index", 18}, {"=[]", 19}, {"struct", 20}, {"struct", 20}, {"value", 21}, {"dec", 22}, {"%", 23}};

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
"includelib \\masm32\\lib\\msvcrt.lib\n";


string pre_define = ".const\n"
"$output_format_int byte \"%d\", 0DH, 0AH, 0\n"
"$input_format_int byte \"%d\", 0\n"
".code\n"
"$print PROC,\n"
"    val: dword\n"
"    invoke crt_printf, addr $output_format_int, val\n"
"    ret\n"
"$print ENDP\n"
"$scan PROC,\n"
"    val: ptr dword\n"
"    invoke crt_scanf, addr $input_format_int,  val\n"
"    ret\n"
"$scan ENDP\n"
"$point PROC,\n"
"    val: ptr dword\n"
"    mov ebx, val\n"
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
    void generateData()
    {
        if(global_id_map.empty())
        {
            return;
        }
        code << "\n\n.data\n";
        map<string, IdValue *>::iterator iter;
        for(iter = global_id_map.begin(); iter != global_id_map.end(); iter++)
        {
            IdValue *id_value  = iter->second;
            string type = "";
            switch(keyMap[iter->second->type])
            {
                // int
                case 6: type = "DWORD"; break;
                // char
                case 7: type = "DWORD"; break;
                // double
                case 8: type = "QWORD"; break;
            }
            if(id_value->is_array)
            {
                code << iter->first << " " << type << " " << id_value->size  << " dup(?)\n";
            }
            else if(id_value->is_pointer)
            {
                code << iter->first << " DWORD ?\n";
            }
            else if(id_value->type == "struct")
            {
                code << iter->first << " " + id_value->struct_name << " ?\n";
            }
            else
            {
                code << iter->first << " " << type << " " << id_value->init_value << "\n";
            }
        }
    }

    void generateStruct()
    {
        if(struct_id_map.empty())
        {
            return;
        }
        code << "\n\n.data\n";
        map<string, StructValue *>::iterator iter;
        for(iter = struct_id_map.begin(); iter != struct_id_map.end(); iter++)
        {
            code << iter->first << " STRUCT\n";
            map<string, IdValue *> *id_type = iter->second->id_type;
            map<string, IdValue *>::iterator iter_t;
            for(iter_t = id_type->begin(); iter_t != id_type->end(); iter_t++)
            {
                IdValue *id_value  = iter_t->second;
                string type = "";
                switch(keyMap[id_value->type])
                {
                    // int
                    case 6: type = "DWORD"; break;
                    // char
                    case 7: type = "DWORD"; break;
                    // double
                    case 8: type = "QWORD"; break;
                }
                if(id_value->is_array)
                {
                    code << iter_t->first << " " << type << " " << id_value->size  << " dup(?)\n";
                }
                else if(id_value->is_pointer)
                {
                    code << iter_t->first << " DWORD ?\n";
                }
                else if(id_value->type == "struct")
                {
                    code << iter_t->first << " " + id_value->struct_name << " ?\n";
                }
                else
                {
                    code << iter_t->first << " " << type << " ?\n";
                }
            }
            code << iter->first << " ENDS\n";
        }
    }

    void generate_local_var(map<string, IdValue *> *id_map)
    {
        if(!id_map->empty())
        {
            map<string, IdValue *>::iterator iter;
            for(iter = id_map->begin(); iter != id_map->end(); iter++) 
            {
                IdValue *id_value  = iter->second;
                string type = "";
                switch(keyMap[id_value->type])
                {
                    // int
                    case 6: type = "DWORD"; break;
                    // char
                    case 7: type = "DWORD"; break;
                    // double
                    case 8: type = "QWORD"; break;
                }
                if(id_value->is_array)
                {
                    code << "    LOCAL " << iter->first << "[" << id_value->size * id_value->width << "]: " << type << "\n";
                }
                else if(id_value->is_pointer)
                {
                    code << "    LOCAL " << iter->first << ": DWORD\n";
                }
                else if(id_value->type == "struct")
                {
                    code << "    LOCAL " << iter->first << ": " + id_value->struct_name << "\n";
                }
                else
                {
                    code << "    LOCAL " << iter->first << ": " << type << "\n";
                }
            }
            code << "\n\n";
        }
    }

    void generate_func_name(string name, SegmentBlock *segment_block)
    {
        code << "$" << name << " proto";
        if(!segment_block->para_map->empty())
        {
            map<string, IdValue *>::iterator iter;
            for(iter = segment_block->para_map->begin(); iter != segment_block->para_map->end(); iter++) 
            {
                code << ",\n    " << iter->first << ": DWORD";
            }
        }
        code << "\n";
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
                    code << "    cdq\n";
                    code << "    mov ebx, " << p->arg2 << "\n";
                    code << "    idiv ebx\n";
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
                    code << "    mov ebx, " << p->result << "\n";
                    break;
                }
                // addr
                case 16: 
                {
                    code << "    INVOKE $point, addr " << p->arg1 << "\n";
                    code << "    mov " << p->result << ", ebx" << "\n";
                    break;
                }
                // []=
                case 17: 
                {
                    code << "    mov eax, " << p->arg1 << "\n";
                    code << "    mov ecx, " << p->arg2 << "\n";
                    code << "    mov " << p->result << "[ecx], eax" << "\n";
                    break;
                }
                // index
                case 18: 
                {
                    code << "    mov ecx, " << p->arg2 << "\n";
                    code << "    mov eax, " << p->arg1 << "[ecx]\n";
                    code << "    mov " << p->result << ", eax" << "\n";
                    break;
                }
                // =[]
                case 19: 
                {
                    code << "    INVOKE $point, addr " << p->arg1 << "[0]\n";
                    code << "    mov " << p->result << ", ebx" << "\n";
                    break;
                }
                // value
                case 21: 
                {
                    code << "    mov eax, DWORD ptr [" << p->arg1 << "+" <<p->arg2 << "]\n";
                    code << "    mov " << p->result << ", eax" << "\n";
                    break;
                }
                // dec
                case 22: 
                {
                    code << "    mov eax, " << p->arg1 << "\n";
                    code << "    dec eax\n";
                    code << "    mov " << p->arg1 << ", eax" << "\n";
                    break;
                }
                // %
                case 23: 
                {
                    code << "    mov eax, " << p->arg1 << "\n";
                    code << "    cdq\n";
                    code << "    mov ebx, " << p->arg2 << "\n";
                    code << "    idiv ebx\n";
                    code << "    mov " << p->result << ", edx" << "\n";
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
        list<string>::iterator iter;
        for(iter = IdList.begin(); iter !=IdList.end(); iter++) 
        {
            SegmentBlock *segment_block = IdTable[*iter];
            ThreeAddress *p = segment_block->begin_address;
            if(p->op == "entry")
            {
                generate_func_name(*iter, segment_block);
            }
        }
        code << pre_define;
        generateData();
        generateStruct();
        code << "\n\n" << ".code" <<"\n";
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
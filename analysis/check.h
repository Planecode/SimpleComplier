#ifndef CHECK_H_INCLUDED
#define CHECK_H_INCLUDED
#include <iostream>
#include "tree.h"
#include <stack>
#include <map>
#include "table.h"
#include <cstdlib>


stack<string> type_stack;

class TypeCheck
{
public:
    SegmentBlock *segment_block;
    TypeCheck(SegmentBlock *segmentBlock):segment_block(segmentBlock)
    {

    }
    ~TypeCheck()
    {

    }
    static void clear_stack()
    {
        while (!type_stack.empty())
        {
            type_stack.pop();
        }
    }
    string compare(string l_type, string r_type)
    {
        if (l_type == "double")
        {
            if (r_type[r_type.size() - 1] == ']' || r_type[r_type.size() - 1] == '*')
                return "ERROR";
            else
                return l_type;
        }
        else if (l_type == "float")
        {
            if (r_type == "double")
                return r_type;
            else if (r_type[r_type.size() - 1] == ']' || r_type[r_type.size() - 1] == '*')
                return "ERROR";
            else
                return l_type;
        }
        else if (l_type == "int")
        {
            if (r_type == "int")
                return l_type;
            else if (r_type[r_type.size() - 1] == ']' || r_type[r_type.size() - 1] == '*')
                return "ERROR";
            else
                return r_type;
        }
        else if (l_type == "char")
        {
            if (r_type != "char")
                return "ERROR";
            else
                return l_type;
        }
        else if (l_type[l_type.size() - 1] == ']')
        {
            return "ERROR";
        }
        else if (l_type[l_type.size() - 1] == '*')
        {
            if (r_type[r_type.size() - 1] == ']' || r_type[r_type.size() - 1] == '*')
                return l_type;
            else
                return "ERROR";
        }
        else
            return "ERROR";

    }
    void check_pointer_assign(node *nowNode)
    {
        if (nowNode->cNode[1]->description == "id")
        {
            cout << "int can not be assigned to int *" << endl;
            exit(-1);
        }
    }
    void check_unary_operator(node *nowNode)
    {
        if (nowNode->description == "r_++" || nowNode->description == "r_--")
        {
            if (nowNode->cNode[0]->description == "id")
            {
                IdValue *id_value = segment_block->get_id_value(nowNode->cNode[0]->value);
                if (id_value->is_array || id_value->is_pointer)
                {
                    cout<< "int * don't support this operator"<<endl;
                    exit(-1);
                }
            }
        }
    }
    void check_binary_operator(node *nowNode)
    {
        string judge = "";
        for (int i = 0; i < nowNode->cNodeLength; i++)
        {
            if (nowNode->cNode[0]->description == "addr")
            {
                cout<< "left expression must be modifiable";
                exit(-1);
            }
            if (nowNode->cNode[i]->description == "id")
            {
                judge = check_id(nowNode->cNode[i], i);
                if (judge != "")
                {
                    cout<< judge<<endl;
                    exit(-1);
                }
            }
            else if (nowNode->cNode[i]->description == "pointer")
            {
                judge = check_pointer_type(nowNode->cNode[i]);
                if (judge != "")
                {
                    cout<< judge<<endl;
                    exit(-1);
                }
            }
            else if (nowNode->cNode[i]->description == "array_id")
            {
                judge = check_array_type(nowNode->cNode[i]);
                if (judge != "")
                {
                    cout<< judge<<endl;
                    exit(-1);
                }
            }
            else if (nowNode->cNode[i]->description == "number" && nowNode->cNode[i]->cNodeLength != 0)
            {
                judge = check_struct(nowNode->cNode[i], i);
                if (judge != "")
                {
                    cout<< judge<<endl;
                    exit(-1);
                }
            }
            if (nowNode->cNode[1]->description == "addr")
            {
                judge = check_id(nowNode->cNode[1], 1);
                if (judge != "")
                {
                    cout<< judge<<endl;
                    exit(-1);
                }
            }
        }
    }
    string check_id(node *nowNode, int i, string struct_id = "")
    {
        IdValue *id_value;
        if (struct_id == "")
        {
            id_value = segment_block->get_id_value(nowNode->value);
        }
        else
        {
            id_value = (*struct_id_map[struct_id]->id_type)[nowNode->value];
        }
        if (id_value->is_array)
        {
            if (type_stack.empty())
            {
                type_stack.push(id_value->type + "[]");
                return "";
            }
            else
            {
                string tmp = type_stack.top();
                type_stack.pop();
                string new_type;
                if (i == 0)
                {  //int a[5];int *p = a;  a = p; ERROR
                    new_type = compare(id_value->type + "[]", tmp);
                }
                else
                {
                    new_type = compare(tmp, id_value->type + "[]");
                }
                if (new_type != "ERROR")
                {
                    type_stack.push(new_type + "[]");
                    return "";
                }
                else
                {
                    return id_value->type + "[]" + " do not support this operator";
                }
                    
            }
        }
        else if (id_value->is_pointer)
        {
            if (type_stack.empty())
            {
                type_stack.push(id_value->type + "*");
                return "";
            }
            else
            {
                string tmp = type_stack.top();
                type_stack.pop();
                string new_type = compare(tmp, id_value->type + "*");
                if (new_type != "ERROR")
                {
                    type_stack.push(new_type + "*");
                    return "";
                }
                else
                {
                    return id_value->type + "*" + " do not support this operator";
                }  
            }
        }
        else
        {
            if (type_stack.empty())
            {
                type_stack.push(id_value->type);
                return "";
            }
            else
            {
                string tmp = type_stack.top();
                type_stack.pop();
                string new_type = compare(tmp, id_value->type);
                if (new_type != "ERROR")
                {
                    type_stack.push(new_type);
                    return "";
                }
                else
                {
                    return id_value->type + " do not support this operator";
                }  
            }
        }
    }
    
    string check_pointer_type(node *nowNode)
    {
        IdValue *id_value;
        node *cNode = nowNode->cNode[0];
        while (1)
        {
            if (cNode->description != "id")
            {
                cNode = cNode->cNode[0];
            }
            else
            {
                id_value = segment_block->get_id_value(cNode->value);
                break;
            }
        }
        if (type_stack.empty())
        {
            type_stack.push(id_value->type);
            return "";
        }
        else
        {
            string tmp = type_stack.top();
            type_stack.pop();
            string new_type = compare(tmp, id_value->type);
            if (new_type != "ERROR")
            {
                type_stack.push(new_type);
                return "";
            }
            else
            {
                return id_value->type + " do not support this operator";
            }
                
        }
    }
    string check_array_type(node *nowNode, string struct_id = "")
    {
        IdValue *id_value;
        if (struct_id == "")
        {
            id_value = segment_block->get_id_value(nowNode->cNode[0]->value);
        }
        else
        {
            id_value = (*struct_id_map[struct_id]->id_type)[nowNode->cNode[0]->value];
        }
        if (type_stack.empty())
        {
            type_stack.push(id_value->type);
            return "";
        }
        else
        {
            string tmp = type_stack.top();
            type_stack.pop();
            string new_type = compare(tmp, id_value->type);
            if (new_type != "ERROR")
            {
                type_stack.push(new_type);
                return "";
            }
            else
            {
                return id_value->type + " do not support this operator";
            }
        }
    }
    string check_struct(node *nowNode, int i)
    {
        string struct_name = segment_block->get_id_value(nowNode->cNode[0]->value)->struct_name;
        if (struct_id_map.count(struct_name) == 0)
        {
            return "strcut " + struct_name + " is not defined";
        }
        map<string, IdValue *> *struct_value = struct_id_map[struct_name]->id_type;
        node *cNode = nowNode->cNode[1];
        if (cNode->description == "id")
        {
            if(struct_value->count(cNode->value) == 0)
            {
                return cNode->value + " is not the member of " + struct_name;
            }
            return check_id(cNode, i, struct_name);
        }
        else if (cNode->description == "array_id")
        {
            if(struct_value->count(cNode->cNode[0]->value) == 0)
            {
                return cNode->cNode[0]->value + " is not the member of " + struct_name;
            }
            return check_array_type(cNode, nowNode->cNode[0]->value);
        }
        else
        {
            return "ERROR EXPRESSION";
        }
    }
};
#endif // CHECK_H_INCLUDED
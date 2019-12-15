#ifndef CHECK_H_INCLUDED
#define CHECK_H_INCLUDED
#include <iostream>
#include "tree.h"
#include <stack>
#include <map>
#include "table.h"

class TypeCheck
{
public:
    SegmentBlock *segment_block;
    stack<string> type_stack;
    TypeCheck(SegmentBlock *segmentBlock):segment_block(segmentBlock)
    {

    }
    ~TypeCheck()
    {

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
    string check_operator(node *nowNode)
    {
        string judge = "";
        for (int i = 0; i < nowNode->cNodeLength; i++)
        {
            if (nowNode->cNode[i]->description == "id")
            {
                judge = check_id(nowNode->cNode[i], i);
                if (judge != "")
                    return judge;
            }
            else if (nowNode->cNode[i]->description == "pointer")
            {
                judge = check_pointer_type(nowNode->cNode[i]);
                if (judge != "")
                    return judge;
            }
            else if (nowNode->cNode[i]->description == "array_id")
            {
                judge = check_array_type(nowNode->cNode[i]);
                if (judge != "")
                    return judge;
            }
            if (nowNode->cNode[1]->description == "=&")
            {
                judge = check_id(nowNode->cNode[1], 1);
                return judge;
            }
        }
        return judge;
    }
    string check_id(node *nowNode, int i)
    {
        IdValue *id_value = segment_block->get_id_value(nowNode->value);
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
                cNode = nowNode->cNode[0];
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
    string check_array_type(node *nowNode)
    {
        IdValue *id_value = segment_block->get_id_value(nowNode->cNode[0]->value);
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
};
#endif // CHECK_H_INCLUDED
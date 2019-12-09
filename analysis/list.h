#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include <iostream>
#include <string>
#include "tree.h"
#include <set>
#include <map>
#include <stack>

using namespace std;

set<string> avoidSet = {"id", "number", "str",  "int", "init_var", "dimension_list", "argv_list"};


class ThreeAddress
{
    public:
    string op;
    string arg1;
    string arg2;
    string result;
    ThreeAddress *next;
    ThreeAddress(string op, string arg1, string arg2, string result):op(op), arg1(arg1), arg2(arg2), result(result), next(0)
    {

    }
    ~ThreeAddress()
    {

    }

};

class ControlJump
{
    public:
    stack<ThreeAddress *> *j_true;
    stack<ThreeAddress *> *j_false;
    ControlJump()
    {
        j_true = new stack<ThreeAddress *>();
        j_false = new stack<ThreeAddress *>();
    }
    ~ControlJump()
    {
        delete j_true;
        delete j_false;
    }
    void jump_true(string label)
    {
        while(this->j_true->size() != 0)
            {
                this->j_true->top()->result = label;
                this->j_true->pop();
            }
    }
    void jump_false(string label)
    {
        while(this->j_false->size() != 0)
            {
                this->j_false->top()->result = label;
                this->j_false->pop();
            }
    }
    void unit_jump(ControlJump *tmp)
    {
        while(tmp->j_true->size() != 0)
            {
                this->j_true->push(tmp->j_true->top());
                tmp->j_true->pop();
            }
            while(tmp->j_false->size() != 0)
            {
                this->j_false->push(tmp->j_false->top());
                tmp->j_false->pop();
            }
    }

};

class List
{
    public:
    ThreeAddress *head;
    ThreeAddress *tail;
    stack<map<string, IdValue *> *> *id_map_stack;
    stack<ThreeAddress *> *enter_address;
    int tmp_seq;
    int label_seq;
    List(): head(0), tail(0), tmp_seq(0), label_seq(0)
    {
        id_map_stack = new stack<map<string, IdValue *> *>();
        enter_address = new stack<ThreeAddress *>();
    }
    ~List()
    {

    }
    string getTmp()
    {
        map<string, IdValue *> *idMap = id_map_stack->top();
        (*idMap)["tmp_" + to_string(tmp_seq)] = new IdValue();
        (*idMap)["tmp_" + to_string(tmp_seq)]->allocate("int");
        return "tmp_" + to_string(tmp_seq++);
    }
    string getLabel()
    {
        return "label_" + to_string(label_seq++);
    }
    void push(ThreeAddress *three_address)
    {
        if(tail == 0)
        {
            head = three_address;
            tail = three_address;
            return;
        }
        if(tail->op == "")
        {
            tail->op = three_address->op;
            tail->arg1 = three_address->arg1;
            tail->arg2 = three_address->arg2;
            tail->result = three_address->result;
            tail->next = three_address->next;
            delete three_address;
            return;
        }
        tail->next = three_address;
        tail = three_address;
    }
    void print()
    {
        ThreeAddress *p = head;
        while(p != 0)
        {
            cout << p->op << " ";
            cout << p->arg1;
            if(p->arg1 != "")
            cout << " ";
            cout << p->arg2;
            if(p->arg2 != "" && p->result != "")
            cout << " ";
            cout << p->result << endl;
            p = p->next;
        }
    }
    void generate(node *root)
    {
        for(int i = 0; i < root->cNodeLength; i++)
        {
            node *cNode = root->cNode[i];
            if(cNode->description == "main_function")
            {
                generate_main(cNode);
            }
        }
    }
    void generate_main(node *nowNode)
    {
        map<string, IdValue *> *idMap = new map<string, IdValue *>();
        id_map_stack->push(idMap);
        enter_address->push(tail);
        for(int i = 0; i < nowNode->cNodeLength; i++)
        {
            node *cNode = nowNode->cNode[i];
            if(cNode == 0)
                continue;
            if(cNode->description == "statement")
                generate_statement(cNode);
        }
    }
    void generate_statement(node *nowNode, ControlJump *control_jump=0)
    {
        for(int i = 0; i < nowNode->cNodeLength; i++)
        {
            node *cNode = nowNode->cNode[i];
            if(cNode->description == "conditional_statement")
                generate_conditional(cNode);
            else if(cNode->description == "while_statement")
                generate_while(cNode);
            else if(cNode->description == "for_statement")
                generate_for(cNode);
            else if(cNode->description == "do_while_statement")
                generate_do_while(cNode);
            else if(cNode->description == "init_var")
                install_id(cNode);
            else if(cNode->description  == "BREAK")
                {
                    ThreeAddress *j_end = new ThreeAddress("J", "", "", "");
                    push(j_end);
                    control_jump->j_true->push(j_end);
                }
            else
                generate_calc(cNode);
        }
    }

    int get_index(node *nowNode)
    {
        map<string, IdValue *> *idMap = id_map_stack->top();
        int index = 0;
        int width = 1;
        for(int i = nowNode->cNode[1]->cNodeLength - 1; i >= 0; i--)
        {
            node *cNode = nowNode->cNode[1]->cNode[i];
            index += stoi(cNode->value) * width;
            width *= (*idMap)[nowNode->cNode[0]->value]->array_width[i];
        }
        return index;
    }
    void generate_calc(node *nowNode)
    {
        for(int i = 0; i < nowNode->cNodeLength; i++)
        {
            node *cNode = nowNode->cNode[i];
            generate_calc(cNode);
        }
        if(avoidSet.count(nowNode->description))
            return;
        if(nowNode->description == "=")
        {
            if(nowNode->cNode[0]->description == "array_id")
            {
                int index = get_index(nowNode->cNode[0]);
                push(new ThreeAddress("array=", nowNode->cNode[1]->value, to_string(index), nowNode->cNode[0]->cNode[0]->value));
            }
            else
            {
                push(new ThreeAddress("=", nowNode->cNode[1]->value, "", nowNode->cNode[0]->value));
            }
        }
        else if(nowNode->description == "array_id")
        {
            int index = get_index(nowNode);
            nowNode->value = getTmp();
            push(new ThreeAddress("index", nowNode->cNode[0]->value, to_string(index), nowNode->value));
        }
        else
        {
            if(nowNode->value == "")
                nowNode->value = getTmp();
            push(new ThreeAddress(nowNode->description, nowNode->cNode[0]->value, nowNode->cNode[1]->value, nowNode->value));
        }       
    }


    void generate_init_assign(node *nowNode)
    {
        generate_calc(nowNode->cNode[1]);
        if(nowNode->cNode[0]->description == "array_id")
        {
            int index = get_index(nowNode->cNode[0]);
            node *argv_list = nowNode->cNode[1];
            for(int i = 0; i < index; i++)
            {
                if(i < argv_list->cNodeLength)
                {
                    push(new ThreeAddress("array=", argv_list->cNode[i]->value, to_string(i), nowNode->cNode[0]->cNode[0]->value));
                }
                else
                {
                    push(new ThreeAddress("array=", "0", to_string(i), nowNode->cNode[0]->cNode[0]->value));
                }
                
            }
        }
        else
        {
            push(new ThreeAddress("=", nowNode->cNode[1]->value, "", nowNode->cNode[0]->value));
        }   
    }

    void install_id(node *nowNode)
    {
        string type = nowNode->cNode[0]->description;
        for(int i = 1; i < nowNode->cNodeLength; i++)
        {
            node *cNode = nowNode->cNode[i];
            local_allocate(cNode, type);
        }
    }
    void local_allocate(node *nowNode, string type)
    {
        map<string, IdValue *> *idMap = id_map_stack->top();
        for(int i = 0; i < nowNode->cNodeLength; i++)
        {
            node *cNode = nowNode->cNode[i];
            local_allocate(cNode, type);
        }
        if(nowNode->description == "id")
        {
            if((*idMap).count(nowNode->value) == 0)
            {
                (*idMap)[nowNode->value] = new IdValue();
            }
            (*idMap)[nowNode->value]->allocate(type);
        }
        else if(nowNode->description == "array_id")
        {
            if((*idMap).count(nowNode->cNode[0]->value) == 0)
            {
                (*idMap)[nowNode->cNode[0]->value] = new IdValue();
            }
            (*idMap)[nowNode->cNode[0]->value]->allocate(type);
            (*idMap)[nowNode->cNode[0]->value]->array_width = new int(nowNode->cNode[1]->cNodeLength);
            for(int i = 0; i < nowNode->cNode[1]->cNodeLength; i++)
            {
                (*idMap)[nowNode->cNode[0]->value]->array_width[i] = stoi(nowNode->cNode[1]->cNode[i]->value);
            }
        }
        else if(nowNode->description == "=")
        {
            generate_init_assign(nowNode);
        }
    }


    void generate_while(node *nowNode)
    {
        string label = getLabel();
        push(new ThreeAddress("label", "", "", label));
        ThreeAddress *j = new ThreeAddress("J", "", "", "");
        j->result = label;
        ControlJump *control_jump = generate_bool_expression(nowNode->cNode[0], "JFalse");
        if(control_jump->j_true->size() != 0)
            {
                string label = getLabel();
                push(new ThreeAddress("label", "", "", label));
                control_jump->jump_true(label);
            }
        
        if(nowNode->cNode[1]->description == "statement")
            generate_statement(nowNode->cNode[1]);
        else
            generate_calc(nowNode->cNode[1]);
        push(j);
        if(control_jump->j_false->size() != 0)
            {
                string label = getLabel();
                push(new ThreeAddress("label", "", "", label));
                control_jump->jump_false(label);
            }
    }
    void generate_for(node *nowNode)
    {

    }

    void generate_do_while(node *nowNode)
    {
        string label = getLabel();
        push(new ThreeAddress("label", "", "", label));
        if(nowNode->cNode[0]->description == "statement")
            generate_statement(nowNode->cNode[0]);
        else
            generate_calc(nowNode->cNode[0]);
        
        ControlJump *control_jump = generate_bool_expression(nowNode->cNode[1], "JTrue");
        if(control_jump->j_true->size() != 0)
        {
            control_jump->jump_true(label);
        }
        if(control_jump->j_false->size() != 0)
        {
            string label = getLabel();
            push(new ThreeAddress("label", "", "", label));
            control_jump->jump_false(label);
        }
    }

    void generate_conditional(node *nowNode)
    {
        if(nowNode->cNode[0]->description == "if_statement")
        {
            if(nowNode->cNodeLength == 1)
            {
                generate_if(nowNode->cNode[0]);
            }
            else
            {
                generate_if(nowNode->cNode[0], nowNode->cNode[1]);
            }
        }
        else
        {
            generate_switch(nowNode->cNode[0]);
        }
            
    }
    void generate_if(node *nowNode, node *elseNode = 0)
    {
        ControlJump *control_jump = generate_bool_expression(nowNode->cNode[0], "JFalse");
        if(control_jump->j_true->size() != 0)
            {
                string label = getLabel();
                push(new ThreeAddress("label", "", "", label));
                control_jump->jump_true(label);
            }
        if(nowNode->cNode[1]->description == "statement")
            generate_statement(nowNode->cNode[1]);
        else
            generate_calc(nowNode->cNode[1]);
        if(control_jump->j_false->size() != 0)
            {
                string label = getLabel();
                push(new ThreeAddress("label", "", "", label));
                control_jump->jump_false(label);
            }
        if(elseNode != 0)
        {
            ThreeAddress *j = new ThreeAddress("J", "", "", "");
            if(tail->op == "label")
                {
                    ThreeAddress *tmp = head;
                    while(tmp->next != tail)
                    {
                        tmp = tmp->next;
                    }
                    tmp->next = j;
                    j->next = tail;
                }
            else
                {
                    push(j);
                }
            generate_statement(elseNode);
            string label = getLabel();
            ThreeAddress *tmp = new ThreeAddress("label", "", "", label);
            push(tmp);
            j->result = label;
        }
    }
    ControlJump *generate_bool_expression(node *nowNode, string end)
    {
        if(nowNode->description == "||")
        {
            ControlJump *control_jump = generate_bool_expression(nowNode->cNode[0], "JTrue");
            if(control_jump->j_false->size() != 0)
            {
                string label = getLabel();
                push(new ThreeAddress("label", "", "", label));
                control_jump->jump_false(label);
            }
            ControlJump * tmp = generate_bool_expression(nowNode->cNode[1], end);
            control_jump->unit_jump(tmp);
            delete tmp;
            return control_jump;
        }
        if(nowNode->description == "&&")
        {
            ControlJump *control_jump = generate_bool_expression(nowNode->cNode[0], "JFalse");
            if(control_jump->j_true->size() != 0)
            {
                string label = getLabel();
                push(new ThreeAddress("label", "", "", label));
                control_jump->jump_true(label);
            }
            ControlJump * tmp = generate_bool_expression(nowNode->cNode[1], end);
            control_jump->unit_jump(tmp);
            delete tmp;
            return control_jump;
        }
        generate_calc(nowNode);
        ThreeAddress *j_end = new ThreeAddress(end + nowNode->description, nowNode->value, "", "");
        push(j_end);
        ControlJump *control_jump = new ControlJump();
        if(end == "JTrue")
            control_jump->j_true->push(j_end);
        else
            control_jump->j_false->push(j_end);
        return control_jump;
    }


    void generate_case(node *nowNode, string cmp)
    {
        ControlJump *control_jump = new ControlJump();
        for(int i = 0; i < nowNode->cNode[0]->cNodeLength; i++)
        {
            node *cNode = nowNode->cNode[0]->cNode[i];
            cNode->value = getTmp();
            push(new ThreeAddress("==", cmp, cNode->cNode[0]->value, cNode->value));
            string label = getLabel();
            push(new ThreeAddress("JTrue==", cNode->value, "", label));
            cNode->value = label;
        }
        ThreeAddress *j = new ThreeAddress("J", "", "", "");
        push(j);
        for(int i = 0; i < nowNode->cNode[0]->cNodeLength; i++)
        {
            node *cNode = nowNode->cNode[0]->cNode[i];
            push(new ThreeAddress("label", "", "", cNode->value));
            generate_statement(cNode->cNode[1], control_jump);
        }
        string label = getLabel();
        push(new ThreeAddress("label", "", "", label));
        j->result = label;
        if(nowNode->cNodeLength == 2)
        {
            generate_statement(nowNode->cNode[1]->cNode[0]);
            if(control_jump->j_true->size() != 0)
            {
                string label = getLabel();
                push(new ThreeAddress("label", "", "", label));
                control_jump->jump_true(label);
            }
        }
        else
        {
            if(control_jump->j_true->size() != 0)
            {
                control_jump->jump_true(label);
            }
        }
        
    }
    void generate_switch(node *nowNode)
    {
        generate_calc(nowNode->cNode[0]);
        generate_case(nowNode->cNode[1], nowNode->cNode[0]->value);
    }
    
};
#endif // LIST_H_INCLUDED

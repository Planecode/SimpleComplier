#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include <iostream>
#include <string>
#include "tree.h"
#include <set>
#include <map>
#include <stack>

using namespace std;

set<string> avoidSet = {"id", "number", "str",  "int", "init_var"};
map<string, IdValue *> idMap;


class ThreeAddress
{
    public:
    string op;
    string arg1;
    string arg2;
    string result;
    ThreeAddress *jump;
    ThreeAddress *next;
    ThreeAddress(string op, string arg1, string arg2, string result):op(op), arg1(arg1), arg2(arg2), result(result), jump(0), next(0)
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
    void jump_true(ThreeAddress *tail)
    {
        while(this->j_true->size() != 0)
            {
                this->j_true->top()->jump = tail;
                this->j_true->pop();
            }
    }
    void jump_false(ThreeAddress *tail)
    {
        while(this->j_false->size() != 0)
            {
                this->j_false->top()->jump = tail;
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
    stack<string> *id_map_stack;
    int tmp_seq;
    List(): head(0), tail(0), tmp_seq(0)
    {
        id_map_stack = new stack<string>();
        id_map_stack->push("");
    }
    ~List()
    {

    }
    string getTmp()
    {
        return "tmp_" + to_string(tmp_seq++);
    }
    string getLocalMap()
    {
        return "local_" + to_string(tmp_seq++);
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
            tail->jump = tail->jump;
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
            cout << "Address->" << p << " " << p->op << " ";
            cout << p->arg1 << " ";
            cout << p->arg2 << " ";
            if(p->result != "")
            {
                    cout << p->result << " ";
            } 
            else
            {
                cout << "Address->" << p->jump << " ";
            }
            cout << endl;
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
        for(int i = 0; i < nowNode->cNodeLength; i++)
        {
            node *cNode = nowNode->cNode[i];
            if(cNode == 0)
                continue;
            if(cNode->description == "statement")
                generate_statement(cNode);
        }
    }
    void generate_statement(node *nowNode)
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
            else
                generate_calc(cNode);
        }
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
            push(new ThreeAddress(nowNode->description, nowNode->cNode[1]->value, "", nowNode->cNode[0]->value));
        else
        {
            if(nowNode->value == "")
                nowNode->value = getTmp();
            push(new ThreeAddress(nowNode->description, nowNode->cNode[0]->value, nowNode->cNode[1]->value, nowNode->value));
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
        for(int i = 0; i < nowNode->cNodeLength; i++)
        {
            node *cNode = nowNode->cNode[i];
            local_allocate(cNode, type);
        }
        if(nowNode->description == "id")
        {
            if(idMap.count(nowNode->value) == 0)
            {
                idMap[nowNode->value] = new IdValue();
            }
            idMap[nowNode->value]->allocate(type);
        }
        else
            generate_calc(nowNode);
    }


    void generate_while(node *nowNode)
    {
        ThreeAddress *tmp = new ThreeAddress("", "", "", "");
        push(tmp);
        ThreeAddress *j = new ThreeAddress("J", "", "", "");
        j->jump = tail;
        ControlJump *control_jump = generate_bool_expression(nowNode->cNode[0], "JFalse");
        if(control_jump->j_true->size() != 0)
            {
                ThreeAddress *tmp = new ThreeAddress("", "", "", "");
                push(tmp);
                control_jump->jump_true(tail);
            }
        
        if(nowNode->cNode[1]->description == "statement")
            generate_statement(nowNode->cNode[1]);
        else
            generate_calc(nowNode->cNode[1]);
        push(j);
        if(control_jump->j_false->size() != 0)
            {
                ThreeAddress *tmp = new ThreeAddress("", "", "", "");
                push(tmp);
                control_jump->jump_false(tail);
            }
    }
    void generate_for(node *nowNode)
    {

    }

    void generate_do_while(node *nowNode)
    {

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
                ThreeAddress *tmp = new ThreeAddress("", "", "", "");
                push(tmp);
                control_jump->jump_true(tail);
            }
        if(nowNode->cNode[1]->description == "statement")
            generate_statement(nowNode->cNode[1]);
        else
            generate_calc(nowNode->cNode[1]);
        if(control_jump->j_false->size() != 0)
            {
                ThreeAddress *tmp = new ThreeAddress("", "", "", "");
                push(tmp);
                control_jump->jump_false(tail);
            }
        if(elseNode != 0)
        {
            ThreeAddress *j = new ThreeAddress("J", "", "", "");
            if(tail->op == "")
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
            ThreeAddress *tmp = new ThreeAddress("", "", "", "");
            push(tmp);
            j->jump = tail;
        }
    }
    ControlJump *generate_bool_expression(node *nowNode, string end)
    {
        if(nowNode->description == "||")
        {
            ControlJump *control_jump = generate_bool_expression(nowNode->cNode[0], "JTrue");
            if(control_jump->j_false->size() != 0)
            {
                ThreeAddress *tmp = new ThreeAddress("", "", "", "");
                push(tmp);
                control_jump->jump_false(tail);
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
                ThreeAddress *tmp = new ThreeAddress("", "", "", "");
                push(tmp);
                control_jump->jump_true(tail);
            }
            ControlJump * tmp = generate_bool_expression(nowNode->cNode[1], end);
            control_jump->unit_jump(tmp);
            delete tmp;
            return control_jump;
        }
        generate_calc(nowNode);
        ThreeAddress *j_end = new ThreeAddress(end, nowNode->value, "", "");
        if(tail->op == "")
        {
            ThreeAddress *tmp = head;
            while(tmp->next != tail)
            {
                tmp = tmp->next;
            }
            tmp->next = j_end;
            j_end->next = tail;
        }
        else
        {
            push(j_end);
        }
        ControlJump *control_jump = new ControlJump();
        if(end == "JTrue")
            control_jump->j_true->push(j_end);
        else
            control_jump->j_false->push(j_end);
        return control_jump;
    }

    void generate_switch(node *nowNode)
    {

    }
};
#endif // LIST_H_INCLUDED

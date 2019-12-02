#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include <iostream>
#include <string>
#include "tree.h"
#include <set>
#include <stack>

using namespace std;

set<string> avoidSet = {"id", "number", "str",  "int", "init_var"};

class ThreeAddress
{
    public:
    string op;
    node *arg1;
    node *arg2;
    node *result;
    ThreeAddress *jump;
    ThreeAddress *next;
    ThreeAddress(string op, node *arg1, node *arg2, node *result):op(op), arg1(arg1), arg2(arg2), result(result), jump(0), next(0)
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

};

class List
{
    public:
    ThreeAddress *head;
    ThreeAddress *tail;
    List(): head(0), tail(0)
    {

    }
    ~List()
    {

    }
    void push(ThreeAddress *three_address)
    {
        if(tail == 0)
        {
            head = three_address;
            tail = three_address;
            return;
        }
        else if(tail->op == "")
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
            cout << "Address->" << p << " " << p->op << " ";
            if(p->arg1 != 0)
            {
                if(p->arg1->description == "id" || p->arg1->description == "str" || p->arg1->description == "number")
                    cout << p->arg1->description << "->" << p->arg1->value << " ";
                else
                    cout << p->arg1 << " ";
            }
            if(p->arg2 != 0)
            {
                if(p->arg2->description == "id" || p->arg2->description == "str" || p->arg2->description == "number")
                    cout << p->arg2->description << "->" << p->arg2->value << " ";
                else
                    cout << p->arg2 << " ";
            }
            if(p->result != 0)
            {
                if(p->result->description == "id" || p->result->description == "str" || p->result->description == "number")
                    cout << p->result->description << "->" << p->result->value << " ";
                else
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
                generate_main(cNode);
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
            push(new ThreeAddress(nowNode->description, nowNode->cNode[1], 0, nowNode->cNode[0]));
        else
            push(new ThreeAddress(nowNode->description, nowNode->cNode[0], nowNode->cNode[1], nowNode));
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
            
    }
    void generate_if(node *nowNode, node *elseNode = 0)
    {
        ControlJump *control_jump = generate_bool_expression(nowNode->cNode[0], "JFalse");
        if(control_jump->j_true->size() != 0)
            {
                ThreeAddress *tmp = new ThreeAddress("", 0, 0, 0);
                push(tmp);
                while(control_jump->j_true->size() != 0)
                {
                    control_jump->j_true->top()->jump = tmp;
                    control_jump->j_true->pop();
                }
            }
        if(nowNode->cNode[1]->description == "statement")
            generate_statement(nowNode->cNode[1]);
        else
            generate_calc(nowNode->cNode[1]);
        if(control_jump->j_false->size() != 0)
            {
                ThreeAddress *tmp = new ThreeAddress("", 0, 0, 0);
                push(tmp);
                while(control_jump->j_false->size() != 0)
                {
                    control_jump->j_false->top()->jump = tmp;
                    control_jump->j_false->pop();
                }
            }
        if(elseNode != 0)
        {
            ThreeAddress *j = new ThreeAddress("J", 0, 0, 0);
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
            ThreeAddress *tmp = new ThreeAddress("", 0, 0, 0);
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
                ThreeAddress *tmp = new ThreeAddress("", 0, 0, 0);
                push(tmp);
                while(control_jump->j_false->size() != 0)
                {
                    control_jump->j_false->top()->jump = tmp;
                    control_jump->j_false->pop();
                }
            }
            ControlJump * tmp = generate_bool_expression(nowNode->cNode[1], end);
            while(tmp->j_true->size() != 0)
            {
                control_jump->j_true->push(tmp->j_true->top());
                tmp->j_true->pop();
            }
            while(tmp->j_false->size() != 0)
            {
                control_jump->j_false->push(tmp->j_false->top());
                tmp->j_false->pop();
            }
            delete tmp;
            return control_jump;
        }
        if(nowNode->description == "&&")
        {
            ControlJump *control_jump = generate_bool_expression(nowNode->cNode[0], "JFalse");
            if(control_jump->j_true->size() != 0)
            {
                ThreeAddress *tmp = new ThreeAddress("", 0, 0, 0);
                push(tmp);
                while(control_jump->j_true->size() != 0)
                {
                    control_jump->j_true->top()->jump = tmp;
                    control_jump->j_true->pop();
                }
            }
            ControlJump * tmp = generate_bool_expression(nowNode->cNode[1], end);
            while(tmp->j_true->size() != 0)
            {
                control_jump->j_true->push(tmp->j_true->top());
                tmp->j_true->pop();
            }
            while(tmp->j_false->size() != 0)
            {
                control_jump->j_false->push(tmp->j_false->top());
                tmp->j_false->pop();
            }
            delete tmp;
            return control_jump;
        }
        generate_calc(nowNode);
        ThreeAddress *j_end = new ThreeAddress(end, nowNode, 0, 0);
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
};
#endif // LIST_H_INCLUDED

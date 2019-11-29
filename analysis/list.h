#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include <iostream>
#include <string>
#include "tree.h"
#include <set>

using namespace std;

set<string> avoidSet = {"id", "number", "str",  "int", "init_var"};

class ThreeAddress
{
    public:
    string op;
    node *arg1;
    node *arg2;
    node *result;
    ThreeAddress *next;
    ThreeAddress(string op, node *arg1, node *arg2, node *result):op(op), arg1(arg1), arg2(arg2), result(result), next(0)
    {

    }
    ~ThreeAddress()
    {

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
        tail->next = three_address;
        tail = three_address;
    }
    void print()
    {
        ThreeAddress *p = head;
        while(p != 0)
        {
            cout << p->op << " ";
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
};
#endif // LIST_H_INCLUDED
#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include <iostream>
#include <string>
#include "tree.h"

using namespace std;

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
};
#endif // LIST_H_INCLUDED
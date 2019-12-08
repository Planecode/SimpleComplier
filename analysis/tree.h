#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <iostream>
#include <string>

using namespace std;

class IdValue
{
    public:
    string type;
    int width;
    int *array_width;
    IdValue(): type("None"), width(0), array_width(0)
    {

    }
    ~IdValue()
    {

    }
    void allocate(string type_name)
    {
        this->type = type_name;
        if(type_name == "int")
        {
            width = 4;
            return;
        }
        if(type_name == "double")
        {
            width = 8;
            return;
        }
    }
};

class node
{
    public:
    string description;
    string value;
    node *(*cNode);
    int cNodeLength;
    node(string description, node *(*cNode), int cNodeLength):description(description), value(""), cNode(cNode), cNodeLength(cNodeLength)
    {

    }
    ~node()
    {

    }
};
typedef node* (*UnitNode);
class tree
{
public:
    node *root;
    tree():root(0)
    {

    }
    ~tree()
    {

    }
    void setRoot(node *nowNode)
    {
        root = nowNode;
    }
    void print(node* nowNode, string block)
    {
        if(nowNode == 0)
        {
            return;
        }
        cout << block;
        if(nowNode != root)
        {
            cout << "|---";
            block += "|   ";
        }
        cout << " " << nowNode->description << " " << nowNode->value;
        cout << endl;
        for(int i = 0; i < nowNode->cNodeLength; i++)
        {
            print(nowNode->cNode[i], block);
        }
        return;
    }
    UnitNode unit_node(node* firstNode, node* secondNode, int mark=0)
    {
        node* singleNode = 0;
        node* mutiNode = 0;
        switch(mark)
        {
            case 0: mutiNode = secondNode;
            singleNode = firstNode;
            break;
            case 1: mutiNode = firstNode;
            singleNode = secondNode;
            break;
        }
        int cNodeLength = 0;
        if(mutiNode != 0)
        {
            cNodeLength = mutiNode->cNodeLength;
        }
        UnitNode newCNode = new (node*[cNodeLength + 1]){};
        if(mark == 0)
        {
            newCNode[0] = singleNode;
            for(int i =0; i < cNodeLength; i++)
            {
                newCNode[i + 1] = mutiNode->cNode[i];
            }
        }
        else if(mark == 1)
        {
            for(int i =0; i < cNodeLength; i++)
            {
                newCNode[i] = mutiNode->cNode[i];
            }
            newCNode[cNodeLength] = singleNode;
        }
        if(mutiNode != 0)
        {
            delete[] mutiNode->cNode;
            delete mutiNode;
        }
        return newCNode;
    }
};

#endif // TREE_H_INCLUDED

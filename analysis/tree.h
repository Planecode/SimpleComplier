#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <iostream>
#include <string>
using namespace std;

class node
{
    string description;
    int value;
public:
    node *(*cNode);
    int cNodeLength;
    node(string description, int value, node *(*cNode), int cNodeLength):description(description), value(value), cNode(cNode), cNodeLength(cNodeLength)
    {

    }
    ~node()
    {

    }
    string getDescription()
    {
        return description;
    }
    int getValue()
    {
        return value;
    }
};

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
    void print(node* nowNode, string block)
    {
        cout << block;
        if(nowNode == 0)
        {
            return;
        }
        if(nowNode != root)
        {
            cout << "|---";
            block += "|   ";
        }
        cout << nowNode->getDescription();
		if(nowNode->getValue() != 0)
		{
			cout << nowNode->getValue();
		}
		cout << endl;
        for(int i = 0; i < nowNode->cNodeLength; i++)
        {
            print(nowNode->cNode[i], block);
        }
        return;
    }
};

#endif // TREE_H_INCLUDED

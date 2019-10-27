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
        cout << " " << nowNode->getDescription() << " ";
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
    UnitNode unit_node(node* singleNode, node* mutiNode)
    {
        int cNodeLength = 0;
        if(mutiNode != 0)
        {
            cNodeLength = mutiNode->cNodeLength;
        }
        UnitNode newCNode = new (node*[cNodeLength + 1]){};
        newCNode[0] = singleNode;
        for(int i =0; i < cNodeLength; i++)
        {
            newCNode[i + 1] = mutiNode->cNode[i];
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

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
    IdValue() : type("None"), width(0)
    {
    }
    ~IdValue()
    {
    }
    void allocate(string type_name)
    {
        this->type = type_name;
        if (type_name == "int")
        {
            width = 4;
            return;
        }
        if (type_name == "double")
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
    node(string description, node *(*cNode), int cNodeLength) : description(description), value(""), cNode(cNode), cNodeLength(cNodeLength)
    {
    }
    ~node()
    {
    }
};
typedef node *(*UnitNode);
class tree
{
public:
    node *root;
    tree() : root(0)
    {
    }
    ~tree()
    {
    }
    void setRoot(node *nowNode)
    {
        root = nowNode;
    }
    void print(node *nowNode, string block)
    {
        if (nowNode == 0)
        {
            return;
        }
        cout << block;
        if (nowNode != root)
        {
            cout << "|---";
            block += "|   ";
        }
        cout << " " << nowNode->description << " " << nowNode->value;
        cout << endl;
        for (int i = 0; i < nowNode->cNodeLength; i++)
        {
            print(nowNode->cNode[i], block);
        }
        return;
    }

    string checkTree(node *nowNode)
    {
        map<string, string> hasValue;    //  define and assigned
        map<string, string> declaration; //  define but may not assigned
        map<string, string>::iterator it;
        string msg = "OK";

        if (nowNode == 0)
        {
            return msg;
        }
        if (nowNode->description == "define_var") //  define and assigned
        {
            hasValue[nowNode->cNode[1]->value] = nowNode->cNode[0]->description;
            //cout<<nowNode->cNode[1]->value<<"   "<<hasValue[nowNode->cNode[1]->value]<<endl;
            return msg;
        }
        else if (nowNode->description == "declaration_var") //  define but may not assigned
        {
            declaration[nowNode->cNode[1]->value] = nowNode->cNode[0]->description;
            return msg;
        }
        else if (nowNode->description == "declaration_struct")
        {
            string name = nowNode->cNode[0]->value;
            node *var_list = nowNode->cNode[1];
            for (int i = 0; i < var_list->cNodeLength; i++)
            {
                node *var = var_list->cNode[i];
                declaration[name + "." + var->cNode[1]->value] = var->cNode[0]->description;
            }
            return msg;
        }
        else if (nowNode->description == "=") // confirm  whether assigned
        {
            if (nowNode->cNode[0]->description == "id")
            {
                string name = nowNode->cNode[0]->value;
                it = hasValue.find(name);
                //cout<<name<<endl;
                //cout<<hasValue[name]<<endl;
                if (it != hasValue.end())
                {
                    return msg;
                }
                it = declaration.find(name);
                if (it != declaration.end())
                {
                    hasValue[name] = declaration[name];
                }
                else
                {
                    return "not define " + name;
                }
            }
            else if (nowNode->cNode[0]->description == "struct_var")
            {

                string name = nowNode->cNode[0]->cNode[0]->value + "." + nowNode->cNode[0]->cNode[1]->value;
                it = hasValue.find(name);
                if (it != hasValue.end())
                {
                    return msg;
                }
                it = declaration.find(name);
                if (it != declaration.end())
                {
                    hasValue[name] = declaration[name];
                    return msg;
                }
                else
                {
                    return "not define " + name;
                }
            }
        }

        else if (nowNode->description == "+" || nowNode->description == "-" || nowNode->description == "*" || nowNode->description == "/" || nowNode->description == "%" || nowNode->description == "^" || nowNode->description == "|" || nowNode->description == "&" || nowNode->description == "||" || nowNode->description == "&&" || nowNode->description == "+=" || nowNode->description == "-=" || nowNode->description == "*=" || nowNode->description == "/=" || nowNode->description == ">>=" || nowNode->description == "<<=" || nowNode->description == "&=" || nowNode->description == "|=" || nowNode->description == "^=" || nowNode->description == "==" || nowNode->description == "!=" || nowNode->description == ">" || nowNode->description == ">=" || nowNode->description == "<" || nowNode->description == "<=" || nowNode->description == "<<" || nowNode->description == ">>") // check whether var is assigned
        {
            if (nowNode->cNode[0]->description == "id")
            {
                string name = nowNode->cNode[0]->value;
                it = hasValue.find(name);
                if (it == hasValue.end())
                {
                    it = declaration.find(name);
                    if (it == declaration.end())
                    {
                        return "not define " + name;
                    }
                    else
                    {
                        return name + " is defined but not assigned";
                    }
                }
            }
            else if (nowNode->cNode[0]->description == "struct_var")
            {
                string name = nowNode->cNode[0]->cNode[0]->value + "." + nowNode->cNode[0]->cNode[1]->value;
                it = hasValue.find(name);
                if (it == hasValue.end())
                {
                    it = declaration.find(name);
                    if (it == declaration.end())
                    {
                        return "not define " + name;
                    }
                    else
                    {
                        return name + " is defined but not assigned";
                    }
                }
            }
            else if (nowNode->cNode[1]->description == "id")
            {
                string name = nowNode->cNode[1]->value;
                it = hasValue.find(name);
                if (it == hasValue.end())
                {
                    it = declaration.find(name);
                    if (it == declaration.end())
                    {
                        return "not define " + name;
                    }
                    else
                    {
                        return name + " is defined but not assigned";
                    }
                }
            }
            else if (nowNode->cNode[1]->description == "struct_var")
            {
                string name = nowNode->cNode[1]->cNode[0]->value + "." + nowNode->cNode[1]->cNode[1]->value;
                it = hasValue.find(name);
                if (it == hasValue.end())
                {
                    it = declaration.find(name);
                    if (it == declaration.end())
                    {
                        return "not define " + name;
                    }
                    else
                    {
                        return name + " is defined but not assigned";
                    }
                }
            }
        }
        cout << nowNode->description << " " << nowNode->value << endl;
        for (int i = 0; i < nowNode->cNodeLength; i++)
        {

            msg = checkTree(nowNode->cNode[i]);
            if (msg != "OK")
            {
                return msg;
            }
        }
        return msg;
    }
    UnitNode unit_node(node *firstNode, node *secondNode, int mark = 0)
    {
        node *singleNode = 0;
        node *mutiNode = 0;
        switch (mark)
        {
        case 0:
            mutiNode = secondNode;
            singleNode = firstNode;
            break;
        case 1:
            mutiNode = firstNode;
            singleNode = secondNode;
            break;
        }
        int cNodeLength = 0;
        if (mutiNode != 0)
        {
            cNodeLength = mutiNode->cNodeLength;
        }
        UnitNode newCNode = new (node * [cNodeLength + 1]){};
        if (mark == 0)
        {
            newCNode[0] = singleNode;
            for (int i = 0; i < cNodeLength; i++)
            {
                newCNode[i + 1] = mutiNode->cNode[i];
            }
        }
        else if (mark == 1)
        {
            for (int i = 0; i < cNodeLength; i++)
            {
                newCNode[i] = mutiNode->cNode[i];
            }
            newCNode[cNodeLength] = singleNode;
        }
        if (mutiNode != 0)
        {
            delete[] mutiNode->cNode;
            delete mutiNode;
        }
        return newCNode;
    }
};

#endif // TREE_H_INCLUDED

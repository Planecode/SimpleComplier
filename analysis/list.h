#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include <iostream>
#include <string>
#include <map>
#include "tree.h"
#include <set>
#include <stack>

using namespace std;

set<string> avoidSet = { "id", "number", "str", "int", "init_var" };
struct ArrayDimension
{
	int dimension[100] = { -1 };
} dimensionList;
class ThreeAddress
{
public:
	string op;
	node *arg1;
	node *arg2;
	node *result;
	ThreeAddress *jump;
	ThreeAddress *next;
	ThreeAddress(string op, node *arg1, node *arg2, node *result) : op(op), arg1(arg1), arg2(arg2), result(result), jump(0), next(0)
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
		while (this->j_true->size() != 0)
		{
			this->j_true->top()->jump = tail;
			this->j_true->pop();
		}
	}
	void jump_false(ThreeAddress *tail)
	{
		while (this->j_false->size() != 0)
		{
			this->j_false->top()->jump = tail;
			this->j_false->pop();
		}
	}
	void unit_jump(ControlJump *tmp)
	{
		while (tmp->j_true->size() != 0)
		{
			this->j_true->push(tmp->j_true->top());
			tmp->j_true->pop();
		}
		while (tmp->j_false->size() != 0)
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
	map<string, ArrayDimension> dimensionList; // store dimension_list to caculate offset
	map<string, IdValue *> idMap;
	map<string, node *> array_address; //store array_address
	List() : head(0), tail(0)
	{
	}
	~List()
	{
	}
	void push(ThreeAddress *three_address)
	{
		if (tail == 0)
		{
			head = three_address;
			tail = three_address;
			return;
		}
		else if (tail->op == "")
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
		while (p != 0)
		{
			cout << "Address->" << p << " " << p->op << " ";
			if (p->arg1 != 0)
			{
				if (p->arg1->description == "id" || p->arg1->description == "str" || p->arg1->description == "number" || p->arg1->description == "offset")
					cout << p->arg1->description << "->" << p->arg1->value << " ";
				else
					cout << p->arg1 << " ";
			}
			if (p->arg2 != 0)
			{
				if (p->arg2->description == "id" || p->arg2->description == "str" || p->arg2->description == "number")
					cout << p->arg2->description << "->" << p->arg2->value << " ";
				else
					cout << p->arg2 << " ";
			}
			if (p->result != 0)
			{
				if (p->result->description == "id" || p->result->description == "str" || p->result->description == "number")
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
		for (int i = 0; i < root->cNodeLength; i++)
		{
			node *cNode = root->cNode[i];
			if (cNode->description == "main_function")
				generate_main(cNode);
		}
	}
	void generate_main(node *nowNode)
	{
		for (int i = 0; i < nowNode->cNodeLength; i++)
		{
			node *cNode = nowNode->cNode[i];
			if (cNode == 0)
				continue;
			if (cNode->description == "statement")
				generate_statement(cNode);
		}
	}
	void generate_statement(node *nowNode)
	{
		for (int i = 0; i < nowNode->cNodeLength; i++)
		{
			node *cNode = nowNode->cNode[i];
			if (cNode->description == "conditional_statement")
				generate_conditional(cNode);
			else if (cNode->description == "while_statement")
				generate_while(cNode);
			else if (cNode->description == "for_statement")
				generate_for(cNode);
			else if (cNode->description == "do_while_statement")
				generate_do_while(cNode);
			else if (cNode->description == "init_var" && cNode->cNode[1]->description == "array_id")
			{
				node *array_id = cNode->cNode[1]->cNode[0];
				node *dim_list = cNode->cNode[1]->cNode[1];
				for (int i = 0; i < dim_list->cNodeLength; i++)
				{
					dimensionList[array_id->value].dimension[i] = stoi(dim_list->cNode[i]->value);
				}
			}
			else
			{
				generate_calc(cNode);
			}
		}
	}
	void generate_calc(node *nowNode)
	{
		for (int i = 0; i < nowNode->cNodeLength; i++)
		{
			node *cNode = nowNode->cNode[i];
			generate_calc(cNode);
		}
		if (avoidSet.count(nowNode->description))
			return;
		if (nowNode->description == "=")
		{
			if (nowNode->cNode[0]->description == "array_id" || nowNode->cNode[1]->description == "array_id")
			{ //array
				generate_array(nowNode);
			}
			else if (nowNode->cNode[0]->description == "pointer")
			{ //pointer
				generate_pointer(nowNode->cNode[0]);
			}
			else if (nowNode->cNode[1]->description == "pointer")
			{
				generate_pointer(nowNode->cNode[1]);
			}
			else
			{ //id
				push(new ThreeAddress(nowNode->description, nowNode->cNode[1], 0, nowNode->cNode[0]));
			}
		}
		else if (nowNode->description == "+" || nowNode->description == "-" || nowNode->description == "*" || nowNode->description == "/" || nowNode->description == "%" || nowNode->description == "^")
		{
			if (nowNode->cNode[0]->description == "array_id" || nowNode->cNode[1]->description == "array_id")
			{ //array
				generate_array(nowNode);
			}
			else if (nowNode->cNode[0]->description == "pointer")
			{ //pointer
				generate_pointer(nowNode->cNode[0]);
			}
			else if (nowNode->cNode[1]->description == "pointer")
			{
				generate_pointer(nowNode->cNode[1]);
			}
			else
			{ //id
				push(new ThreeAddress(nowNode->description, nowNode->cNode[0], nowNode->cNode[1], nowNode));
			}
		}
	}
	void generate_local_calc(node *nowNode, map<string, IdValue *> &localIdMap)
	{
		for (int i = 0; i < nowNode->cNodeLength; i++)
		{
			node *cNode = nowNode->cNode[i];
			if (cNode->description == "init_var")
			{
				local_install_id(cNode, localIdMap);
			}
			generate_local_calc(cNode, localIdMap);
		}
		if (avoidSet.count(nowNode->description))
			return;
		if (nowNode->description == "=")
		{
			if (nowNode->cNode[0]->description == "array_id" || nowNode->cNode[1]->description == "array_id")
			{ //array
				generate_array(nowNode);
			}
			else if (nowNode->cNode[0]->description == "pointer")
			{ //pointer
				generate_pointer(nowNode->cNode[0]);
			}
			else if (nowNode->cNode[1]->description == "pointer")
			{
				generate_pointer(nowNode->cNode[1]);
			}
			else
			{ //id
				push(new ThreeAddress(nowNode->description, nowNode->cNode[1], 0, nowNode->cNode[0]));
			}
		}
		else
		{
			if (nowNode->cNode[0]->description == "array_id" || nowNode->cNode[1]->description == "array_id")
			{ //array
				generate_array(nowNode);
			}
			else if (nowNode->cNode[0]->description == "pointer")
			{ //pointer
				generate_pointer(nowNode->cNode[0]);
			}
			else if (nowNode->cNode[1]->description == "pointer")
			{
				generate_pointer(nowNode->cNode[1]);
			}
			else
			{ //id
				push(new ThreeAddress(nowNode->description, nowNode->cNode[0], nowNode->cNode[1], nowNode));
			}
		}
	}
	void local_install_id(node *nowNode, map<string, IdValue *> &localIdMap)
	{
		string type = nowNode->cNode[0]->description;
		for (int i = 1; i < nowNode->cNodeLength; i++)
		{
			node *cNode = nowNode->cNode[i];
			local_allocate(cNode, localIdMap, type);
		}
	}
	void local_allocate(node *nowNode, map<string, IdValue *> &localIdMap, string type)
	{
		for (int i = 1; i < nowNode->cNodeLength; i++)
		{
			node *cNode = nowNode->cNode[i];
			local_allocate(cNode, localIdMap, type);
		}
		if (nowNode->description == "id")
		{
			if (localIdMap.count(nowNode->value) == 0)
			{
				localIdMap[nowNode->value] = new IdValue();
			}
			localIdMap[nowNode->value]->allocate(type);
		}
	}
	void generate_array(node *nowNode)
	{
		if (nowNode->description == "=")
		{
			if (nowNode->cNode[0]->description == "array_id" && nowNode->cNode[1]->description != "array_id")
			{
				node *offset = add_array_address(nowNode->cNode[0]);
				if (offset != 0)
				{
					push(new ThreeAddress("+", offset, nowNode->cNode[0]->cNode[0], nowNode->cNode[0]));
					push(new ThreeAddress("=", nowNode->cNode[1], 0, nowNode->cNode[0]));
				}
				else
				{
					cout << nowNode->cNode[0]->value + offset->value << "is not assigned" << endl;
				}
			}
			else if (nowNode->cNode[0]->description != "array_id" && nowNode->cNode[1]->description == "array_id")
			{
				string array_id = nowNode->cNode[0]->cNode[0]->value;
				string offset_num = check_array_address(nowNode->cNode[1]);
				if (offset_num != "ERROR")
				{
					push(new ThreeAddress("=", array_address[array_id + offset_num], 0, nowNode->cNode[0]));
				}
			}
			else if (nowNode->cNode[0]->description == "array_id" && nowNode->cNode[1]->description == "array_id")
			{
				node *offset = add_array_address(nowNode->cNode[0]);
				if (offset != 0)
				{
					push(new ThreeAddress("+", offset, nowNode->cNode[0]->cNode[0], nowNode->cNode[0]));
					push(new ThreeAddress("=", nowNode->cNode[1], 0, nowNode->cNode[0]));
				}
				else
				{
					cout << nowNode->cNode[0]->value + offset->value << "is not assigned" << endl;
					return;
				}
				string array_id = nowNode->cNode[0]->cNode[0]->value;
				string offset_num = check_array_address(nowNode->cNode[1]);
				if (offset_num != "ERROR")
				{
					push(new ThreeAddress("=", array_address[array_id + offset_num], 0, array_address[array_id + offset->value]));
				}
			}
		}
		else
		{
			if (nowNode->cNode[0]->description == "array_id" && nowNode->cNode[1]->description != "array_id")
			{
				string array_id = nowNode->cNode[1]->cNode[0]->value;
				string offset_num = check_array_address(nowNode->cNode[0]);
				if (offset_num != "ERROR")
				{
					push(new ThreeAddress(nowNode->description, array_address[array_id + offset_num], nowNode->cNode[0], nowNode));
				}
			}
			else if (nowNode->cNode[0]->description != "array_id" && nowNode->cNode[1]->description == "array_id")
			{
				string array_id = nowNode->cNode[1]->cNode[0]->value;
				string offset_num = check_array_address(nowNode->cNode[1]);
				if (offset_num != "ERROR")
				{
					push(new ThreeAddress(nowNode->description, nowNode->cNode[0], array_address[array_id + offset_num], nowNode));
				}
			}
			else if (nowNode->cNode[0]->description == "array_id" && nowNode->cNode[1]->description == "array_id")
			{
				string array_id1 = nowNode->cNode[0]->cNode[0]->value;
				string array_id2 = nowNode->cNode[1]->cNode[0]->value;
				string offset_num1 = check_array_address(nowNode->cNode[0]);
				string offset_num2 = check_array_address(nowNode->cNode[1]);
				if (offset_num1 != "ERROR" && offset_num2 != "ERROR")
				{
					push(new ThreeAddress(nowNode->description, array_address[array_id1 + offset_num1], array_address[array_id2 + offset_num2], nowNode));
				}
			}
		}
	}
	int get_offset(string id, node *list)
	{ //argv: array_id  dimension_list
		if (dimensionList.count(id) == 0)
		{
			cout << "array " + id + "is not defined" << endl;
		}
		int offset = 0;
		for (int i = 0; i < list->cNodeLength; i++)
		{
			int tmp = stoi(list->cNode[i]->value) + 1;
			if (tmp <= dimensionList[id].dimension[i])
			{ // not out of range
				for (int j = i + 1; j < list->cNodeLength; j++)
				{
					tmp = tmp * dimensionList[id].dimension[j];
				}
				offset = offset + tmp;
			}
		}
		return offset - 1;
	}
	node *add_array_address(node *nowNode) //node array_id
	{                                      // add array_address to array_address map
		string array_id = nowNode->cNode[0]->value;
		if (dimensionList.count(array_id) != 0)
		{
			int offset_num = get_offset(array_id, nowNode->cNode[1]) * idMap[array_id]->width;
			node *offset = new node("offset", 0, 0);
			offset->value = to_string(offset_num);
			if (array_address.count(array_id + offset->value) == 0)
			{
				array_address[array_id + offset->value] = nowNode;
			}
			return offset;
		}
		else
		{
			return 0;
		}
	}
	string check_array_address(node *nowNode)
	{ //check array_address is exist and return offset
		string array_id = nowNode->cNode[0]->value;
		int offset_num = get_offset(array_id, nowNode->cNode[1]) * idMap[array_id]->width;
		if (array_address.count(array_id + to_string(offset_num)) != 0)
		{
			return to_string(offset_num);
		}
		else
		{
			cout << array_id + to_string(offset_num) << " is not assign" << endl;
			return "ERROR";
		}
	}
	void generate_pointer(node *nowNode)
	{
	}

	void generate_while(node *nowNode)
	{
		map<string, IdValue *> localIdMap;
		ThreeAddress *tmp = new ThreeAddress("", 0, 0, 0);
		push(tmp);
		ThreeAddress *j = new ThreeAddress("J", 0, 0, 0);
		j->jump = tail;
		ControlJump *control_jump = generate_bool_expression(nowNode->cNode[0], "JFalse");
		if (control_jump->j_true->size() != 0)
		{
			ThreeAddress *tmp = new ThreeAddress("", 0, 0, 0);
			push(tmp);
			control_jump->jump_true(tail);
		}

		if (nowNode->cNode[1]->description == "statement")
			generate_statement(nowNode->cNode[1]);
		else
			generate_local_calc(nowNode->cNode[1], localIdMap);
		push(j);
		if (control_jump->j_false->size() != 0)
		{
			ThreeAddress *tmp = new ThreeAddress("", 0, 0, 0);
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
		if (nowNode->cNode[0]->description == "if_statement")
		{
			if (nowNode->cNodeLength == 1)
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
		map<string, IdValue *> localIdMap;
		ControlJump *control_jump = generate_bool_expression(nowNode->cNode[0], "JFalse");
		if (control_jump->j_true->size() != 0)
		{
			ThreeAddress *tmp = new ThreeAddress("", 0, 0, 0);
			push(tmp);
			control_jump->jump_true(tail);
		}
		if (nowNode->cNode[1]->description == "statement")
			generate_statement(nowNode->cNode[1]);
		else
			generate_local_calc(nowNode->cNode[1], localIdMap);
		if (control_jump->j_false->size() != 0)
		{
			ThreeAddress *tmp = new ThreeAddress("", 0, 0, 0);
			push(tmp);
			control_jump->jump_false(tail);
		}
		if (elseNode != 0)
		{
			ThreeAddress *j = new ThreeAddress("J", 0, 0, 0);
			if (tail->op == "")
			{
				ThreeAddress *tmp = head;
				while (tmp->next != tail)
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
		if (nowNode->description == "||")
		{
			ControlJump *control_jump = generate_bool_expression(nowNode->cNode[0], "JTrue");
			if (control_jump->j_false->size() != 0)
			{
				ThreeAddress *tmp = new ThreeAddress("", 0, 0, 0);
				push(tmp);
				control_jump->jump_false(tail);
			}
			ControlJump *tmp = generate_bool_expression(nowNode->cNode[1], end);
			control_jump->unit_jump(tmp);
			delete tmp;
			return control_jump;
		}
		if (nowNode->description == "&&")
		{
			ControlJump *control_jump = generate_bool_expression(nowNode->cNode[0], "JFalse");
			if (control_jump->j_true->size() != 0)
			{
				ThreeAddress *tmp = new ThreeAddress("", 0, 0, 0);
				push(tmp);
				control_jump->jump_true(tail);
			}
			ControlJump *tmp = generate_bool_expression(nowNode->cNode[1], end);
			control_jump->unit_jump(tmp);
			delete tmp;
			return control_jump;
		}
		generate_calc(nowNode);
		ThreeAddress *j_end = new ThreeAddress(end, nowNode, 0, 0);
		if (tail->op == "")
		{
			ThreeAddress *tmp = head;
			while (tmp->next != tail)
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
		if (end == "JTrue")
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

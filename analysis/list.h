#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include <iostream>
#include <string>
#include "tree.h"
#include <set>
#include <map>
#include <stack>
#include <cstdlib>
#include "check.h"
#include "table.h"

using namespace std;

set<string> avoidSet = {"id", "number", "str",  "int", "init_var", "dimension_list", "argv_list", "NULL"};
map<string, string> bool_jump = {
    {"JTrue>", "jg"}, {"JTrue==", "je"}, {"JTrue<", "jl"}, {"JTrue>=", "jge"}, {"JTrue<=", "jle"}, {"JTrue!=", "jne"}, 
    {"JFalse>", "jle"}, {"JFalse==", "jne"}, {"JFalse<", "jge"}, {"JFalse>=", "jl"}, {"JFalse<=", "jg"}, {"JFalse!=", "je"}, 
    };

set<string> cmpSet{">", ">=", "<=", "<", "=="};

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
    void switch_true_false()
    {
        stack<ThreeAddress *> *tmp = j_true;
        j_true = j_false;
        j_false = tmp;
    }
};

class List
{
    public:
    SegmentBlock *segment_block;
    ThreeAddress *head;
    ThreeAddress *tail;
    int tmp_seq;
    int label_seq;
    List(): head(0), tail(0), tmp_seq(0), label_seq(0), segment_block(0)
    {

    }
    ~List()
    {

    }
    string getTmp()
    {
        IdValue *id_value = segment_block->install_id("tmp_" + to_string(tmp_seq));
        id_value->allocate("int");
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
    void safe_push(string op, node *node_1, node *node_2, node *node_3)
    {
        string arg1 = "";
        if(node_1 != 0)
        {
            if(node_1->description != "number")
            {
                arg1 = segment_block->get_true_id(node_1->value);
            }
            else
            {
                arg1 = node_1->value;
            }
            
        }
        string arg2 = "";
        if(node_2 != 0)
        {
            if(node_2->description != "number")
            {
                arg2 = segment_block->get_true_id(node_2->value);
            }
            else
            {
                arg2 = node_2->value;
            }
            
        }
        string result = "";
        if(node_3 != 0)
        {
            result = segment_block->get_true_id(node_3->value);   
        }
        push(new ThreeAddress(op, arg1, arg2, result));
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
        string label = getLabel();
        push(new ThreeAddress("entry", "", "", label));
        if(IdTable.count("main") == 0)
        {
            segment_block = new SegmentBlock(tail);
            IdTable["main"] = segment_block;
        }
        for(int i = 0; i < nowNode->cNodeLength; i++)
        {
            node *cNode = nowNode->cNode[i];
            if(cNode == 0)
                continue;
            if(cNode->description == "statement")
                generate_statement(cNode);
        }
        segment_block->end_address = tail;
    }
    void generate_statement(node *nowNode, ControlJump *control_jump=0)
    {
        if(nowNode->description == "statement")
        {
            for(int i = 0; i < nowNode->cNodeLength; i++)
            {
                node *cNode = nowNode->cNode[i];
                generate_statement(cNode);
            }
        }
        else if(nowNode->description == "conditional_statement")
            generate_conditional(nowNode);
        else if(nowNode->description == "while_statement")
            generate_while(nowNode);
        else if(nowNode->description == "for_statement")
            generate_for(nowNode);
        else if(nowNode->description == "do_while_statement")
            generate_do_while(nowNode);
        else if(nowNode->description == "init_var")
            install_id(nowNode);
        else if(nowNode->description  == "BREAK")
            {
                ThreeAddress *j_end = new ThreeAddress("J", "", "", "");
                push(j_end);
                control_jump->j_true->push(j_end);
            }
        else
        {
            generate_calc(nowNode);
        }
    }

    int get_index(node *nowNode)
    {
        int index = 0;
        int width = 1;
        IdValue *id_value = segment_block->get_id_value(nowNode->cNode[0]->value);
        for(int i = nowNode->cNode[1]->cNodeLength - 1; i >= 0; i--)
        {
            node *cNode = nowNode->cNode[1]->cNode[i];
            index += stoi(cNode->value) * width;
            width *= id_value->array_width[i];
        }
        return index;
    }
    void generate_pointer_expression(node *nowNode)
    {
        if(nowNode->cNode[0]->description == "id")
        {
            nowNode->value = nowNode->cNode[0]->value;
        }
    }
    void generate_array_expression(node *nowNode)
    {
        int index = get_index(nowNode);
        nowNode->value = getTmp();
        string arg1 = segment_block->get_true_id(nowNode->cNode[0]->value);
        string result = segment_block->get_true_id(nowNode->value);
        push(new ThreeAddress("index", arg1, to_string(index), result));
    }
    void generate_calc(node *nowNode)
    {
        for(int i = 0; i < nowNode->cNodeLength; i++)
        {
            node *cNode = nowNode->cNode[i];
            if(cNode->description == "pointer")
            {
                generate_pointer_expression(cNode);
                continue;
            }
            if(cNode->description == "array_id")
            {
                generate_array_expression(cNode);
                continue;
            }
            generate_calc(cNode);
        }
        if(avoidSet.count(nowNode->description))
            return;
        
        if(nowNode->description == "=")
        {
            TypeCheck type_check(segment_block);
            string msg = type_check.check_operator(nowNode);
            if(msg != "")
            {
                cout << msg << endl;
                exit(-1);
            }
            if(nowNode->cNode[0]->description == "array_id")
            {
                int index = get_index(nowNode->cNode[0]);
                string arg1 = segment_block->get_true_id(nowNode->cNode[1]->value);
                string result = segment_block->get_true_id(nowNode->cNode[0]->cNode[0]->value);
                push(new ThreeAddress("[]=", arg1, to_string(index), result));
            }
            else if (nowNode->cNode[0]->description == "pointer")
            {
                safe_push("p=", nowNode->cNode[1], 0, nowNode->cNode[0]);
            }
            else
            {
                safe_push("=", nowNode->cNode[1], 0, nowNode->cNode[0]);
            }
            
        }
        else if(nowNode->description == "r_++")
        {
            safe_push("inc", nowNode->cNode[0], 0, 0);
        }
        else if(cmpSet.count(nowNode->description))
        {
            safe_push("cmp", nowNode->cNode[0], nowNode->cNode[1], 0);
        }
        else
        {
            TypeCheck type_check(segment_block);
            string msg = type_check.check_operator(nowNode);
            if(msg != "")
            {
                cout << msg << endl;
                exit(-1);
            }
            if (nowNode->value == "")
            {
                nowNode->value = getTmp();
            }
            safe_push(nowNode->description, nowNode->cNode[0], nowNode->cNode[1], nowNode);
        }
    }



    void local_allocate(node *nowNode, string type)
    {
        if(nowNode->description == "id")
        {
            IdValue *id_value = segment_block->install_id(nowNode->value);
            id_value->allocate(type);
        }
        else if(nowNode->description == "array_id")
        {
            IdValue *id_value = segment_block->install_id(nowNode->cNode[0]->value);
            id_value->is_array = 1;
            id_value->dimension = nowNode->cNode[1]->cNodeLength;
            id_value->array_width = new int(nowNode->cNode[1]->cNodeLength);
            for(int i = 0; i < nowNode->cNode[1]->cNodeLength; i++)
            {
                id_value->array_width[i] = stoi(nowNode->cNode[1]->cNode[i]->value);
            }
            id_value->allocate(type);
        }
        else if(nowNode->description == "pointer")
        {
            int dimension = 1;
            node *cNode = nowNode->cNode[0];
            while(cNode->description != "id")
            {
                cNode = cNode->cNode[0];
                dimension++;
            }
            IdValue *id_value = segment_block->install_id(cNode->value);
            id_value->is_pointer = 1;
            id_value->dimension = dimension;
            id_value->allocate(type);
        }
    }
    void generate_init_assign(node *nowNode, string type)
    {
        local_allocate(nowNode->cNode[0], type);
        if(nowNode->cNode[0]->description == "array_id")
        {
            int index = get_index(nowNode->cNode[0]);
            node *argv_list = nowNode->cNode[1];
            for(int i = 0; i < index; i++)
            {
                if(i < argv_list->cNodeLength)
                {
                    string arg1 = argv_list->cNode[i]->value;
                    if(argv_list->cNode[i]->description != "number")
                    {
                        arg1 = segment_block->get_true_id(argv_list->cNode[i]->value);
                    }
                    string result = segment_block->get_true_id(nowNode->cNode[0]->cNode[0]->value);
                    push(new ThreeAddress("[]=", arg1, to_string(i), result));
                }
                else
                {
                    string result = segment_block->get_true_id(nowNode->cNode[0]->cNode[0]->value);
                    push(new ThreeAddress("[]=", "0", to_string(i), result));
                }
                
            }
        }
        else if (nowNode->cNode[0]->description == "pointer")
        {
            string arg1 = nowNode->cNode[1]->value;
            if(nowNode->cNode[1]->description != "number")
            {
                arg1 = segment_block->get_true_id(nowNode->cNode[1]->value);
            }
            node *cNode = nowNode->cNode[0]->cNode[0];
            while(cNode->description != "id")
            {
                cNode = cNode->cNode[0];
            }
            string result = segment_block->get_true_id(cNode->value);
            push(new ThreeAddress("p=", arg1, "", result));
        }
        else
        {
            generate_calc(nowNode->cNode[1]);
            string arg1 = nowNode->cNode[1]->value;
            if(nowNode->cNode[1]->description != "number")
            {
                arg1 = segment_block->get_true_id(nowNode->cNode[1]->value);
            }
            string result = segment_block->get_true_id(nowNode->cNode[0]->value);
            push(new ThreeAddress("=", arg1, "", result));
        }
    }
    void install_id(node *nowNode)
    {
        string type = nowNode->cNode[0]->description;
        for(int i = 1; i < nowNode->cNodeLength; i++)
        {
            node *cNode = nowNode->cNode[i];
            if(cNode->description == "=")
            {
                generate_init_assign(cNode, type);
            }
            else
            {
                local_allocate(cNode, type);
            }
        }
    }


    void generate_while(node *nowNode)
    {
        segment_block->add_depth();
        string label = getLabel();
        push(new ThreeAddress("label", "", "", label));
        ThreeAddress *j = new ThreeAddress("jmp", "", "", "");
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
        segment_block->remove_depth();
    }
    void generate_for(node *nowNode)
    {
        segment_block->add_depth();
        generate_statement(nowNode->cNode[0]);
        string label = getLabel();
        push(new ThreeAddress("label", "", "", label));
        ThreeAddress *j = new ThreeAddress("jmp", "", "", "");
        j->result = label;
        ControlJump *control_jump = generate_bool_expression(nowNode->cNode[1], "JFalse");
        if(control_jump->j_true->size() != 0)
        {
            string label = getLabel();
            push(new ThreeAddress("label", "", "", label));
            control_jump->jump_true(label);
        }
        generate_statement(nowNode->cNode[3]);
        generate_statement(nowNode->cNode[2]);
        push(j);
        if(control_jump->j_false->size() != 0)
        {
            string label = getLabel();
            push(new ThreeAddress("label", "", "", label));
            control_jump->jump_false(label);
        }
        segment_block->remove_depth();
    }

    void generate_do_while(node *nowNode)
    {
        segment_block->add_depth();
        string label = getLabel();
        push(new ThreeAddress("label", "", "", label));
        generate_statement(nowNode->cNode[0]);
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
        segment_block->remove_depth();
    }

    void generate_conditional(node *nowNode)
    {
        segment_block->add_depth();
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
        segment_block->remove_depth();
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
        generate_statement(nowNode->cNode[1]);
        if(control_jump->j_false->size() != 0)
            {
                string label = getLabel();
                push(new ThreeAddress("label", "", "", label));
                control_jump->jump_false(label);
            }
        if(elseNode != 0)
        {
            ThreeAddress *j = new ThreeAddress("jmp", "", "", "");
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
        else if(nowNode->description == "&&")
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
        else if(nowNode->description == "!")
        {
            if(end == "JTrue")
            {
                end = "JFalse";
            }
            else
            {
                end = "JTrue";
            }
            ControlJump * control_jump = generate_bool_expression(nowNode->cNode[0], end);
            control_jump->switch_true_false();
            return control_jump;
        }
        generate_calc(nowNode);
        ThreeAddress *j_end = new ThreeAddress(bool_jump[end + nowNode->description], "", "", "");
        push(j_end);
        ControlJump *control_jump = new ControlJump();
        if(end == "JTrue")
        {
            control_jump->j_true->push(j_end);
        }
        else
        {
            control_jump->j_false->push(j_end);
        }
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

#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <set>
#include "tree.h"

map<string, IdValue *> global_id_map;
map<string, StructValue *> struct_id_map;
class ThreeAddress
{
    public:
    string op;
    string arg1;
    string arg2;
    string result;
    ThreeAddress *next;
    ThreeAddress(string op, string arg1, string arg2, string result):op(op), arg1(arg1), arg2(arg2), result(result), next(0)
    {

    }
    ~ThreeAddress()
    {

    }

};

class LocalSet
{
    public:
    int seq;
    LocalSet *next;
    LocalSet *front;
    set<string> *local_set;
    LocalSet(int seq, LocalSet *front): seq(seq), next(0), front(front)
    {
        local_set = new set<string>();
    }
    ~LocalSet()
    {
        delete local_set;
    }
};

class SegmentBlock
{
    public:
    int stack_seq;
    LocalSet *local_head;
    LocalSet *local_tail;
    map<string, IdValue *> *id_map;
    map<string, IdValue *> *para_map;
    ThreeAddress *begin_address;
    ThreeAddress *end_address;
    SegmentBlock(ThreeAddress *begin_address): stack_seq(0), begin_address(begin_address), end_address(0)
    {
        local_head = new LocalSet(0, 0);
        local_tail = local_head;
        id_map = new map<string, IdValue *>();
        para_map = new map<string, IdValue *>();
    }
    ~SegmentBlock()
    {
        delete id_map;
    }
    IdValue *install_id(string id_name)
    {
        string install_name = id_name + "_" + to_string(local_tail->seq);
        if(id_map->count(install_name) == 0)
        {
            (*id_map)[install_name] = new IdValue();
            return (*id_map)[install_name];
        }
        return 0;
    }
    IdValue *install_para_id(string id_name)
    {
        if(para_map->count(id_name) == 0)
        {
            (*para_map)[id_name] = new IdValue();
            return (*para_map)[id_name];
        }
        return 0;
    }
    IdValue *get_id_value(string id)
    {
        LocalSet *p = local_tail;
        while(p != 0)
        {
            string install_name = id + "_" + to_string(p->seq);
            if(id_map->count(install_name) != 0)
            {
                return (*id_map)[install_name];
            }
            p = p->front;
        }
        if(para_map->count(id) != 0)
        {
            return (*para_map)[id];
        }
        if(global_id_map.count(id) != 0)
        {
            return global_id_map[id];
        }
        return 0;
    }
    string get_true_id(string id)
    {
        LocalSet *p = local_tail;
        while(p != 0)
        {
            string install_name = id + "_" + to_string(p->seq);
            if(id_map->count(install_name) != 0)
            {
                return install_name;
            }
            p = p->front;
        }
        return id;
    }
    void add_depth()
    {
        local_tail->next = new LocalSet(++stack_seq, local_tail);
        local_tail = local_tail->next;
    }
    void remove_depth()
    {
        local_tail = local_tail->front;
        LocalSet *p = local_tail->next;
        local_tail->next = 0;
        delete  p;
    }
};

map<string, SegmentBlock *> IdTable;
list<string> IdList;

#endif // TABLE_H_INCLUDED

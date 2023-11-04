#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <fstream>
#include <mutex>
#include <iostream>
#include <string>
#include "node.h"
#include "node.cpp"

//SkipList类
template <typename K, typename V>
class SkipList{
public:
    SkipList(int);
    SkipList();
    ~SkipList();

    int get_random_level();
    Node<K, V>* create_node(K, V, int);
    bool insert_element(const K, const V);
    void display_list();
    bool search_element(K, V&);
    bool delete_element(K);
    void dump_file(std::string path);
    void load_file(std::string path);
    int size();

private:
    void get_key_value_from_string(const std::string& str, std::string& key, str::string& value);
    bool is_valid_string(const std::string & str);
private:
    //Maximum level of the skip list
    int _max_level;

    //current level of skip list
    int _skip_list_level;

    //file operator
    std::ofstream _file_writer;
    std::ifstream _file_reader;
    std::mutex mtx;

    //skiplist current element count
    int _element_count;

    //pointer to header node
    Node<K, V> *_header;

};


#endif // SKIPLIST_H

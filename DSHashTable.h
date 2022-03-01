/*
 Created by jadon on 11/9/2021.
 reference: Mark Fontenot
*/


#ifndef INC_21F_FINAL_PROJ_TEMPLATE_DSHASHTABLE_H
#define INC_21F_FINAL_PROJ_TEMPLATE_DSHASHTABLE_H

#include <iostream>
using namespace std;

/* essentially a key accessed adjacency list*/
template<typename K, typename V>
class DSHashTable {

private:
    struct Node {
        K key;
        V value;

        Node() = default;
        explicit Node(const K& arg1, const V& arg2): key(arg1), value(arg2) {}
    };

    vector<Node> *data{};
    int capacity;
    int size;

    /*contains every node pushed into table despite index*/
    vector<Node> traversed;

    void resize() {
        capacity *= 2;
        auto* temp = new vector<Node>[capacity];
        for (int i = 0; i < size; i++) {
            temp[i] = data[i];
        }
        delete[] data;
        data = temp;
    }

public:

/*    hash table is a dynamically allocated array where
     each index of array contains a vector of key,value pairs */
    DSHashTable() {
        capacity = 1000081; //1mil and 81
        size = 0;
        data = new vector<Node>[capacity];
    }

    DSHashTable(const DSHashTable<K, V>& rhs) {
        size = rhs.size;
        capacity = rhs.capacity;
        data = new vector<Node>[capacity];
        //each vector at each index of dyn. array is copied over
        for (int i = 0; i < size; i++)
            data[i] = rhs[i];
    }

    DSHashTable<K,V>& operator=(const DSHashTable<K,V>& rhs){
        if (this == &rhs) {
            return *this;
        }
        delete[] data;
        size = rhs.size;
        capacity = rhs.capacity;
        data = new vector<Node>[capacity];
        for (int i = 0; i < size; i++)
            data[i] = rhs[i];
        return *this;
    }

    ~DSHashTable() { delete data; }

    /*returns number of indices with populated vectors*/
    int Size() { return size; }
    /*return bool whether table has been populated or not*/
    bool is_empty() { return size == 0; }

    /*inserts into table by hashing key to produce index of dyn. array,
     then inserts key and value as pair into the vector at that index
    modding key by capacity finds index within the range of the hash table*/
    void insert(const K& key, const V& value) {
        if (size == capacity) {
            resize();
        }
        std::hash<K> hashing;
        int index = hashing(key) % capacity;
        if (data[index].size() == 0) {
            size++;
        }
        data[index].push_back(Node(key, value));
        traversed.push_back(Node(key, value));
    }

    /*returns vector at index*/
    vector<Node>& operator[](int index) const {
        return data[index];
    }

    /*removes node from the vector at the index of hashed key*/
    void remove(const K& key, const V& value) {
        std::hash<K> hashing;
        int index = hashing(key) % capacity;
        for (int i = 0; i < data[index].size(); i++) {
            if (data[index].at(i).key == key) {
                data[index].at(i).erase();
            }
        }
    }

    /*receives key and returns vector of pair objects at index of the key*/
    vector<Node>& find(const K& key) {
        std::hash<K> hashing;
        int index = hashing(key) % capacity;
        return data[index];
    }

/*
    receives key and returns boolean if info is stored for that key
*/
    bool contains(const K& key) {
        std::hash<K> hashing;
        int index = hashing(key) % capacity;
        return (data[index].size() != 0);
    }


    vector<Node>& getTraversed(){
        return traversed;
    }
};


#endif //INC_21F_FINAL_PROJ_TEMPLATE_DSHASHTABLE_H

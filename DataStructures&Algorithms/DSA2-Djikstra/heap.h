#ifndef _HEAP_H
#define _HEAP_H

#include <vector>
#include <string>
#include "hash.h"

using namespace std;

class heap
{
public:
    heap(int capacity);
    int insert(const std::string &id, int key, void *pv = nullptr);
    int setKey(const std::string &id, int key);
    int deleteMin(std::string *pId = nullptr, int *pKey = nullptr, void *ppData = nullptr);
    int remove(const std::string &id, int *pKey = nullptr, void *ppData = nullptr);

private:
    class node
    { // An inner class within heap
    public:
        string id; // The id of this node
        int key; // The key of this node
        void *pData; // A pointer to the actual data
    };

    // private data members
    int capacity;       // capacity of heap
    int size;           // current size of heap
    vector<node> data;  // The actual binary heap
    hashTable *mapping; // maps ids to node pointers


    void percolateUp(int posCur);   // Move a key higher up in tree
    void percolateDown(int posCur); // Move key down the tree
    int getPos(node *pn);           // get position of node
};

#endif











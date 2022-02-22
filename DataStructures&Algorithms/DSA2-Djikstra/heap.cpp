#include "heap.h"
#include <iostream>
#include <climits>

using namespace std;

// Initialize
heap::heap(int capacity)
{
    data.resize(capacity+1);
    mapping = new hashTable(capacity * 2);
    this->capacity = capacity;
    size = 0;
}

// Insert
int heap::insert(const std::string &id, int key, void *pv)
{

    if (!(mapping->contains(id)))
        {
            size ++;
            data[size].id = id;
            data[size].key = key;
            data[size].pData = pv;
            mapping->insert(id, &data[size]);
            percolateUp(size);
        }
    if (size == capacity)
    {
        return 1;
    }
    else
    {
            return 2;
    }
    return 0;
}

//setKey
int heap::setKey(const std::string &id, int key)
{
    if (!(mapping->contains(id)))
        return 1;
    int old, pos;
    node *pn = static_cast<node *>(mapping->getPointer(id));
    pos = getPos(pn);
    old = pn->key;
    pn->key = key;

    if (key > old)
    {
        percolateDown(pos);
    }
    else if (key < old)
    {
        percolateUp(pos);
    }
    return 0;
}

//deleteMin
int heap::deleteMin(std::string *pId, int *pKey, void *ppData)
{
    if (size == 0)
        return 1;

    else
    {
        node min = data[1];
        mapping->remove(min.id);
        if (pKey != nullptr) { *pKey = min.key; }
        if (pId != nullptr) { *pId = min.id; }
        if (ppData != nullptr) { *(static_cast<void **>(ppData)) = data[1].pData; }

        data[1] = data[size--];
        //data[size+1] = node();
        percolateDown(1);

        return 0;
    }
}

// remove
int heap::remove(const std::string &id, int *pKey, void *ppData)
{
    if (!(mapping->contains(id)))
        return 1;
    node *pn = static_cast<node *>(mapping->getPointer(id));
    if (ppData != nullptr)
        *(static_cast<void **>(ppData)) = pn->pData;
    if (pKey != nullptr)
        *pKey = pn->key;

    setKey(id, INT_MIN);
    deleteMin();
    return 0;
}

void heap::percolateUp(int posCur)
{
    data[0] = data[posCur];
    node inserted = data[posCur];
    for (; posCur > 1 && inserted.key < data[posCur / 2].key; posCur /= 2)
    {
        data[posCur] = data[posCur / 2];
        mapping->setPointer(data[posCur].id, &data[posCur]);
    }
    data[posCur] = data[0];
    mapping->setPointer(data[posCur].id, &data[posCur]);
}


// percolate down
void heap::percolateDown(int posCur)
{
    int child;
    node removed = data[posCur];
    for (; posCur * 2 <= size; posCur = child)
    {
        child = posCur * 2;
        if (child != size && data[child + 1].key < data[child].key)
            child++;
        if (data[child].key < removed.key)
        {
            data[posCur] = data[child];
            mapping->setPointer(data[posCur].id, &data[posCur]);
        }
        else
            break;
    }
    data[posCur] = removed;
    mapping->setPointer(data[posCur].id, &data[posCur]);
}


int heap::getPos(node *pn)
{
    return (pn - &data[0]);
}


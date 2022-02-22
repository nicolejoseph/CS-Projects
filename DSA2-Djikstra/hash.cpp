
#include "hash.h"
#include <iostream>
#include <cmath>
#include <string>
#include <typeinfo>
using namespace std;

// Hash table is created
// Constructor initializes hash table with a certain capacity
hashTable::hashTable(int size)
{
    capacity = getPrime(size);
    data.resize(capacity);
    filled = 0;

    for (int i = 0; i < data.size(); i++)
    {
        data[i].isOccupied = false;
        data[i].isDeleted = false;
    }
}

// Hash Function: djb2
// Source: http://www.cse.yorku.ca/~oz/hash.html
int hashTable::hash(const std::string &key)
{
    unsigned long hash = 5381;
    int c;
    int i = 0;
    while(c = key[i++])
        hash = ((hash << 5) + hash) + c;
    return hash % capacity;
}

// Insert specified key into hash table, check for existing key, rehash if necessary
int hashTable::insert(const std::string &key, void *pv)
{
    int check;
    if ((filled) >= ((capacity)/2))
        check = rehash();
    if (check == 2)
        {
            return 2;
        }

    int pos = hash(key);
    if (contains(key) == true)
    {
        return 1;
    }
    while ((data[pos].isOccupied || data[pos].isDeleted) && (data[pos].key != key))
    {
        pos++;
        if (pos == capacity)
            pos = 0;
    }

    data[pos].key = key;
    data[pos].pv = pv;
    data[pos].isOccupied = true;
    data[pos].isDeleted = false;
    filled++;
    return 0;
}

// Check if a specified value exists in the hash table
bool hashTable::contains(const std::string &key)
{
    if(findPos(key) >= 0) {
        return true;
    }
    else
    {
        return false;
    }

}

// Find the position of a specified value
int hashTable::findPos(const std::string &key)
{
    int pos = hash(key);
    while (data[pos].isOccupied)
    {
        if (data[pos].key == key)
        {
            if (data[pos].isDeleted == false)
                return pos;
        }
        else
        {
                pos++;
                if (pos == capacity)
                    pos = 0;
        }
    }
    return -1;
    }


// Rehash function to rehash current data and expand hash table capacity
bool hashTable::rehash()
{
    vector<hashItem> backupData = data;
    int newSize = getPrime(capacity);
    try
    {
        data.resize(newSize);
    }
    catch (const bad_alloc)
    {
        cout << "Rehash Failed" << endl;
        return false;
    }
    capacity = newSize;
    filled = 0;
    for (int i = 0; i < data.size(); i++)
    {
        data[i].key = "";
        data[i].isOccupied = false;
        data[i].isDeleted = false;
    }
    for (int i = 0; i < backupData.size(); i++)
    {
        hashItem oldPoint = backupData[i];
        if (oldPoint.isOccupied && !oldPoint.isDeleted)
        {
            insert(oldPoint.key, oldPoint.pv);
        }
    }
    return 1;
}

// Source for Prime List: https://catonmat.net/tools/generate-random-prime-numbers
unsigned int hashTable::getPrime(int size)
{
    static int primeList[] = {37, 1297, 160243, 388499, 767359, 1706491, 2517869, 4411061, 5334281, 6400013};
    unsigned int primeNum;
    for (int i = 0; i < sizeof(primeList) / sizeof(int); i++)
    {
        if (primeList[i] >= (size * 2))
        {
            primeNum = primeList[i];
            break;
        }
    }
    return primeNum;
}

void *hashTable::getPointer(const std::string &key, bool *b)
{
    int pos;
    if ((pos = findPos(key)) >= 0)
    {
        if (b != NULL)
        {
            *b = true;
        }
        return data[pos].pv;
    }
    if (b != NULL)
    {
        *b = false;
    }
    return NULL;
}

int hashTable::setPointer(const std::string &key, void *pv)
{
    int pos;
    if ((pos = findPos(key)) >= 0)
    {
        data[pos].pv = pv;
        return 0;
    }
    return 1;
}

bool hashTable::remove(const std::string &key)
{
    int pos = findPos(key);
    if (pos == -1)
    {
        return false;
    }
    else
    {
        data[pos].isOccupied = false;
        data[pos].isDeleted = true;
        return true;
    }
}
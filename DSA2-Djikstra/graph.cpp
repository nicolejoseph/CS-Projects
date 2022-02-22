#include <iostream>
#include <sstream>
#include <iterator>
#include "graph.h"
using namespace std;

graph::graph()
{
    vertices = new hashTable(1000000);
    capacity = 0;
}

void graph::loadGraph(std::ifstream &in)
{
    int distance;
    string vertex1, vertex2;
    while (in >> vertex1 >> vertex2 >> distance)
    {
        insert(vertex1, vertex2, distance);
    }
}

// If vertex doesn't exist, it's added to the hash table
void graph::insert(const std::string &vertex1, const std::string &vertex2, int distance)
{
    vertex *temp1;
    vertex *temp2;
    edge newEdge;

    if (!vertices -> contains(vertex1))
    {
        temp1 = new vertex;
        temp1 -> vertexID = vertex1;
        temp1 -> known = false;
        temp1 -> distance = INT_MAX;        // INT_MAX used for infinity
        vertexList.push_back(temp1);
        vertices -> insert(vertex1, temp1);
        capacity++; }
    else
    {
        temp1 = (vertex *)vertices -> getPointer(vertex1);
    }

    if (!vertices -> contains(vertex2))
    {
        temp2 = new vertex;
        temp2 -> vertexID = vertex2;
        temp2 -> known = false;
        temp2 -> distance = INT_MAX;
        vertexList.push_back(temp2);
        vertices -> insert(vertex2, temp2);
        capacity++; }
    else
    {
        temp2 = (vertex *)vertices -> getPointer(vertex2);
    }

//  The edge will point to vertex2 and be inserted into the adjacency list of vertex1
    newEdge.destVertex = temp2;
    newEdge.cost = distance;
    temp1 -> adj.push_back(newEdge);
}

bool graph::dijkstra(const std::string &startingV)
{

    vertex *start = (vertex *)vertices -> getPointer(startingV);
    start -> distance = 0;
    start -> path.push_back(startingV);
    heap graphHeap(capacity);
    graphHeap.insert(startingV, start -> distance, start);

    for (std::list<vertex *>::const_iterator it = vertexList.begin(), end = vertexList.end();it != end; ++it) {
        if ((*it) -> vertexID != startingV)
        {
            graphHeap.insert((*it) -> vertexID, (*it) -> distance, *it);
        }
    }

    vertex *temp = nullptr;
    for (int i = 0; i < capacity; ++i)
    {
        graphHeap.deleteMin(nullptr, nullptr, &temp);
        temp -> known = true;
        for (std::list<edge>::const_iterator it = temp -> adj.begin(), end = temp -> adj.end(); it  != end && temp -> distance != INT_MAX; ++it) {
            if ( (!it -> destVertex -> known) && (it -> destVertex -> distance > (it -> cost + temp -> distance)) &&  (temp -> distance != INT_MAX) )
            {
                it -> destVertex -> path.clear();
                it -> destVertex -> path.insert(it -> destVertex -> path.begin(), temp -> path.begin(), temp -> path.end());
                it -> destVertex -> path.push_back(it -> destVertex -> vertexID);
                it -> destVertex -> distance = (it -> cost + temp -> distance);
                graphHeap.setKey(it -> destVertex -> vertexID, (it -> cost + temp -> distance));
            }
        }

    }
    return true;
}

bool graph::checkVertex(const std::string &vertex)
{
    return vertices -> contains(vertex);
}

void graph::outputGraph(std::ofstream &out)
{
    for (std::list<vertex*>::const_iterator it = vertexList.begin(); it != vertexList.end(); ++it)
    {
        out << (*it) -> vertexID << ": ";
        if ((*it) -> distance == INT_MAX)
        {
            out << "NO PATH" << std::endl;
        }
        else
        {
            out << (*it) -> distance << " [";
            for (std::list<string>::const_iterator i = (*it) -> path.begin(); i!= (*it) -> path.end(); ++i) {
                out << *i;
                if (std::next(i, 1)!= (*it) -> path.end()) {
                    out << ", ";
                }
            }
            out << "]" << std::endl;
        }
    }
}
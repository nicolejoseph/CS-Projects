#ifndef _GRAPH_H
#define _GRAPH_H

#include <climits>
#include <fstream>
#include <string>
#include <vector>
#include <list>

#include "hash.h"
#include "heap.h"
using namespace std;

// size of graph
// function to load in graph from input file
// function that inserts the vertices info into hashtable
// function that checks if a vertex is already knwon
// function for dijkstra implementation
// function that prints shortest paths of graph into output file

class graph
{
public:
    graph();

    int capacity;
    void loadGraph(std::ifstream &in);
    void insert(const std::string &vertex1, const std::string &vertex2, int distance);
    bool checkVertex(const std::string &vertex);
    bool dijkstra(const std::string &startingV);
    void outputGraph(std::ofstream &out);

private:
    // private nested class to store nodes of the graph
    class vertex;

    struct edge
    {
    public:
        int cost;
        vertex* destVertex;
    };

    class vertex
    {
    public:
        std::string vertexID;
        std::list<edge> adj;
        bool known;
        int distance;
        std::list<std::string> path;
    };

    std::list<vertex*> vertexList;
    hashTable * vertices;
};

#endif
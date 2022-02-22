#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include "graph.h"

using namespace std;

int main()
{
    std::string infile;
    std::string outfile;
    std::string startingV;
    ifstream input;
    ofstream output;
    graph gph;

    while (!input.is_open())
    {
        cout << "Enter a valid graph input file:" << endl;
        cin >> infile;
        input.open(infile, ifstream::in);
        if(!input.is_open())
        {
            cout << "INVALID INPUT. Please try again." << endl;
        }
    }

    gph.loadGraph(input);
    input.close();

    while(!gph.checkVertex(startingV))
    {
        cout << "Enter a valid starting vertex:" << endl;
        cin >> startingV;
        if(!gph.checkVertex(startingV))
            cout << "INVALID INPUT. Please try again." << endl;
    }
    clock_t begin = clock();
    gph.dijkstra(startingV);
    clock_t end = clock();
    double CPUtime = ((double)(end - begin)) / CLOCKS_PER_SEC;
    cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << CPUtime << endl;

    while(!output.is_open())
    {
        cout << "Enter name of output file:" << endl;
        cin >> outfile;
        output.open(outfile, ofstream::out | ofstream::trunc);
    }

    gph.outputGraph(output);
    output.close();
    return 0;
}

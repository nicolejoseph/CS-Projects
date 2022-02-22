#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class neuralNet {
public:
    vector<int> layerSize;
    neuralNet(ifstream &load);
    double sig(double x);
    void test(ifstream &testingData, ofstream &fout);

    class neuron;

    class edge{
    public:
        neuron *layerN;
        double weight;
    };

    class neuron {
    public:
        double input;
        double activation;
        vector<edge> edgesIn;
        vector<edge> edgesOut;
    };
    vector<vector<neuron>> layers;

    class trainData {
    public:
        vector<double> in;
        vector<int> out;
    };
};

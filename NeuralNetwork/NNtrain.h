// helpful source: https://vimeo.com/19569529

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class neuralNet {
public:
    vector<int> layerSize;
    neuralNet(ifstream &load);
    void train(ifstream &trainingData, double learnRate, int epoch,ostream &fout);
    double sig(double x);
    double sigDeriv(double x);

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
        double error;
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
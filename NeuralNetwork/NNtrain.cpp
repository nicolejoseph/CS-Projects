// helpful source: https://vimeo.com/19569529

#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include "./NNtrain.h"

#define layerNum 3
using namespace std;

neuralNet::neuralNet(ifstream& load)
{
    double temp;
    edge in, out;
    layers.resize(layerNum);
    layerSize.resize(layerNum);

    for(int i = 0; i < 3; i++)
    {
        load >> layerSize[i];
        layers[i].resize(++(layerSize[i]));
    }

    for(int i = 0; i < 3; i++)
    {
        layers[i][0].activation = -1;
    }

    for(int i = 0; i < 2; i++)
    {
        for(int j = 1; j < layerSize[i + 1]; j++)
        {
            for(int k = 0; k < layerSize[i]; k++)
            {
                load >> temp;

                in.weight = temp;
                in.layerN = &layers[i][k];
                out.weight = temp;
                out.layerN = &layers[i + 1][j];

                layers[i + 1][j].edgesIn.push_back(in);
                layers[i][k].edgesOut.push_back(out);
            }
        }
    }
}

double neuralNet::sig(double x)
{
    return 1.0/(1.0 + exp(-x));
}
double neuralNet::sigDeriv(double x)
{
    return this->sig(x)*(1.0 - (this->sig(x)));
}

void neuralNet::train(ifstream &trainingData, double learnRate, int epoch, ostream &fout)
{
    double sum;
    int cols, inNum, outNum;
    const int outId = layerNum - 1;

    fout << setprecision(3) << fixed;
    trainingData >> cols >> inNum >> outNum;
    vector<trainData> trains(cols);

    for(int i = 0; i < cols; i++)
    {
        trains[i].in.resize(inNum);
        trains[i].out.resize(outNum);

        for(int j = 0; j < outNum; j++)
        {
            trainingData >> trains[i].out[j];
        }
        for(int j = 0; j < inNum; j++)
        {
            trainingData >> trains[i].in[j];
        }
    }

    for(int i = 0; i < epoch; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            for(int k = 0; k < inNum; k++)
            {
                layers[0][k + 1].activation = trains[j].in[k];
            }
            for(int k = 1; k < 3; k++)
            {
                for(int m = 1; m < layerSize[k]; m++)
                {
                    layers[k][m].input = 0;
                    for(auto &it:layers[k][m].edgesIn)
                    {
                        layers[k][m].input += it.weight * it.layerN->activation;
                    }
                    layers[k][m].activation = this -> sig(layers[k][m].input);
                }
            }

            for(int k = 1;k < layerSize[outId]; k++)
            {
                layers[outId][k].error = this -> sigDeriv(layers[outId][k].input)
                        *(trains[j].out[k - 1] - layers[outId][k].activation);
            }

            for(int k = outId - 1; k > 0; k--)
            {
                for(int m = 1; m < layerSize[k]; m++)
                {
                    sum = 0;
                    for(auto &it:layers[k][m].edgesOut)
                    {
                        sum += it.weight * it.layerN -> error;
                    }
                    layers[k][m].error = sum * this -> sigDeriv(layers[k][m].input);
                }
            }
            for(int k = 1; k < 3; k++)
            {
                for(int m = 1; m < layerSize[k]; m++)
                {
                    for(auto &it:layers[k][m].edgesIn)
                    {
                        it.weight = learnRate * it.layerN->activation*layers[k][m].error + it.weight;
                        it.layerN -> edgesOut[m - 1].weight = it.weight;
                    }
                }
            }
        }
    }

    int index = 0;
    for (int i = 0; i < 3; i++)
    {
        if(index++) fout << " ";
        fout << this -> layerSize[i] - 1;
    }
    fout << endl;

    for(int i = 1; i < 3; i++)
    {
        for(int j = 1; j < this -> layerSize[i]; j++)
        {
            index = 0;
            for(auto &it:layers[i][j].edgesIn)
            {
                if(index) fout << " ";
                fout << it.weight;
                index++;
            }
            fout << endl;
        }
    }
}


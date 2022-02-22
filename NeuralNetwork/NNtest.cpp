#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include "./NNtest.h"

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

void neuralNet::test(ifstream& testingData, ofstream& fout)
{
    int cols, inNum, outNum;
    double A = 0, B = 0, C = 0, D = 0;
    double accuracy, precision, recall, F1;
    double accuracyAvg, precisionAvg, recallAvg, F1Avg;

    fout << setprecision(3) << fixed;
    testingData >> cols >> inNum >> outNum;
    vector<trainData> tests(cols);
    vector<vector<double>> predic(outNum);
    const int outId = layerNum - 1;

    for(int i = 0; i < cols; i++)
    {
        tests[i].in.resize(inNum);
        tests[i].out.resize(outNum);

        for(int j = 0; j < inNum; j++)
        {
            testingData >> tests[i].in[j];
        }
        for(int j = 0; j < outNum; j++)
        {
            testingData >> tests[i].out[j];
        }
    }
    for(int i = 0; i < outNum; i++)
    {
        predic[i].resize(4);
        for(int j = 0;j < 4; j++)
            predic[i][j] = 0;
    }

    for(int i = 0; i < cols; i++)
    {
        for(int j = 0; j < inNum; j++)
        {
            layers[0][j + 1].activation = tests[i].in[j];
        }
        for(int j = 1; j < layerNum; j++)
        {
            for(int k = 1; k < layerSize[j]; k++)
            {
                layers[j][k].input = 0;
                for(auto &it:layers[j][k].edgesIn)
                    layers[j][k].input += it.weight * it.layerN -> activation;
                layers[j][k].activation = this -> sig(layers[j][k].input);
            }
        }

        /* ********** Setting up for confusion matrix and computation of A, B, C, D metrics ********** */
        for(int j = 0; j < (layerSize[outId] - 1); j++)
        {
            if(layers[outId][j + 1].activation >= 0.5)
            {
                if(tests[i].out[j])
                    predic[j][0]++;
                else
                    predic[j][1]++;
            }
            else
            {
                if(tests[i].out[j])
                    predic[j][2]++;
                else
                    predic[j][3]++;
            }
        }
    }

    for(int i = 0; i < outNum; i++)
    {
        A += predic[i][0];
        B += predic[i][1];
        C += predic[i][2];
        D += predic[i][3];
        accuracy = (predic[i][0] + predic[i][3])/(predic[i][0] + predic[i][1] + predic[i][2] + predic[i][3]);
        precision = predic[i][0]/(predic[i][0] + predic[i][1]);
        recall = predic[i][0]/(predic[i][0] + predic[i][2]);
        F1 = (2 * precision * recall)/(precision + recall);
        accuracyAvg += accuracy;
        precisionAvg += precision;
        recallAvg += recall;
        fout << (int)predic[i][0] << " " << (int)predic[i][1] << " " << (int)predic[i][2] << " " << (int)predic[i][3]
        << " " << accuracy << " " << precision << " " << recall << " " << F1 << endl;
    }

    accuracy = (A + D)/(A + B + C + D);
    precision = A/(A + B);
    recall = A/(A + C);
    F1 = (2 * precision * recall)/(precision + recall);
    accuracyAvg/=outNum;
    precisionAvg/=outNum;
    recallAvg/=outNum;
    F1Avg = (2 * precisionAvg * recallAvg)/(precisionAvg + recallAvg);

    fout << accuracy << " " << precision << " " << recall << " " << F1 << endl;
    fout << accuracyAvg << " " << precisionAvg << " " << recallAvg << " " << F1Avg << endl;
}
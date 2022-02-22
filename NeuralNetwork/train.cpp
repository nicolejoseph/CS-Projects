#include <iostream>
#include <fstream>
#include "./NNtrain.h"

using namespace std;

int main()
{
    string temp1, temp2, temp;
    int epoch;
    double learnRate;
    ifstream initial, ftrain;
    ofstream output;

    cout << "Enter file name of the initial neural net: ";
    cin >> temp1;
    initial.open(temp1);
    temp1.clear();

    cout << "Enter file name of the training set: ";
    cin >> temp2;
    ftrain.open(temp2);
    temp2.clear();

    cout << "Enter file name for the trained output file: ";
    cin >> temp;
    output.open(temp);

    cout << "Enter number of epochs:";
    cin >> epoch;
    cout << "Enter the learning rate: ";
    cin >> learnRate;

    if(initial.is_open() && ftrain.is_open() && output.is_open())
    {
        neuralNet *NNtrain = new neuralNet(initial);
        NNtrain -> train(ftrain, learnRate, epoch,output);
    }

    return 0;
}

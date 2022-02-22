#include <iostream>
#include <fstream>
#include "./NNtest.h"

using namespace std;

int main()
{
    string temp1, temp2, temp;
    ifstream trained, ftest;
    ofstream output;

    cout << "Enter file name of the trained neural network: ";
    cin >> temp1;
    trained.open(temp1);
    temp1.clear();

    cout << "Enter file name of the test set: ";
    cin >> temp2;
    ftest.open(temp2);
    temp2.clear();

    cout << "Enter file name for the final output: ";
    cin >> temp;
    output.open(temp);

    if(trained.is_open() && ftest.is_open() && output.is_open())
    {
        neuralNet *NNtest = new neuralNet(trained);
        NNtest -> test(ftest, output);
    }
    return 0;
}


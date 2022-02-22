#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#define MAX 1001

using namespace std;

// helpful sources: https://www.geeksforgeeks.org/find-if-a-string-is-interleaved-of-two-other-strings-dp-33/
// https://afteracademy.com/blog/interleaving-strings
// https://slaystudy.com/check-if-the-given-string-is-interleaving-of-two-other-strings/

class matrix{
public:
    string A, B, C;
    int direction;
    int index;
    matrix(int idx, int dir): index(idx), direction(dir){}
};

string A, B, C;
int i, j;

const matrix defset(0, 9);
vector<matrix> x(MAX, defset);
vector< vector<matrix> > IL(MAX, x);

bool isInterleaved(string &A, string &B, string &C, ofstream &output)
{
    int L1 = A.length();
    int L2 = B.length();
    int L3 = C.length();

   if ((A.length() + B.length()) != C.length() || (C[C.length() - 1] != A[A.length() - 1] && C[C.length() - 1] != B[B.length() - 1]))
    {
        output << "*** NOT A MERGE ***" << endl;
    }
    else
    {
        A = " " + A;
        B = " " + B;
        C = " " + C;

        for(j = 0; j < B.length() + 1; j++)
        {
            for (i = 0; i < A.length() + 1; i++)
            {
                if ( i > 0 && C[i + j] == A[i] && IL[i - 1][j].index + 1 == i + j)
                {
                    IL[i][j].index = IL[i - 1][j].index + 1;
                    IL[i][j].direction = 0;
                }
                if (j > 0 && C[i + j] == B[j] && IL[i][j - 1].index + 1 == i + j)
                {
                    IL[i][j].index = IL[i][j - 1].index + 1;
                    IL[i][j].direction = 1;
                }
            }
        }

        i = L1;
        j = L2;
        if (IL[i][j].direction == 9)
        {
            output << "*** NOT A MERGE ***" << endl;
        }

        else
        {
            while (i > 0 || j > 0)
            {
                if (IL[i][j].direction == 0)
                {
                    C[i + j] = toupper(C[i + j]);
                    i--;
                }
                else if (IL[i][j].direction == 1)
                {
                    j--;
                }
            }
            output << C.substr(1, L3) << endl;
        }
    }

    for(int j = 0; j < MAX; j++)
    {
        for (int i = 0; i < MAX; i++)
        {
            IL[i][j] = defset;
        }
    }
    return false;
}

int main()
{
    string infile, outfile;
    // string A, B, C;

    std::cout << "Enter name of input file: ";
    std::cin >> infile;
    std::cout << "Enter name of output file: ";
    std::cin >> outfile;

    ifstream input(infile.c_str());
    ofstream output(outfile.c_str());

    if(!input.is_open() || !output.is_open()){
        if(!input.is_open())
        {
            fprintf(stderr, "ERROR. Can not open input file\n", strerror(errno));
        }
        else if(!output.is_open())
        {
            fprintf(stderr, "ERROR. Can not open output file\n", strerror(errno));
        }
        input.close(); output.close();
        return -1;
    }

    while (input >> A)
    {
        input >> B;
        input >> C;
        isInterleaved(A, B, C, output);
    }

    input.close();
    output.close();
    return 0;
}

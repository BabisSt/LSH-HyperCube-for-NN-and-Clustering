#pragma once

#include <fstream>

using namespace std;

enum Mode
{
    _lsh,
    _cube,
    _cluster
};

class Input
{
    private:
        char *getCmdOption(char **begin, char **end, const string &option);
    
    public:
        int nClusters;          //for cluster
        int lsh_k, L;           //for lsh
        int cube_k, M, probes;  //for hypercube
        int N, R;               //for lsh and hypercube
        char *method = nullptr;
        bool complete = false;
        Mode mode;
        ifstream inputFile, queryFile;
        ofstream outputFile;

        Input();
        ~Input();

        int parseCmdOptions(const int &argc, char *argv[]);

        int OpenInputFile(const string &file);
        int OpenQueryFile(const string &file);

};
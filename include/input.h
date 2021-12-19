
/*
Auto to arxeio einai gia to input poy dinei o xrhsths
Bazei ola auta pou dinei o xrhsths se mia class
kai ta diaxeirizetai apo ekei
*/
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
    int nClusters;         // for cluster
    int lsh_k, L;          // for lsh
    int cube_k, M, probes; // for hypercube
    int N, R;              // for lsh and hypercube
    char *method = nullptr;
    bool complete = false;
    Mode mode;
    ifstream inputFile, queryFile;
    ofstream outputFile;

    Input();
    ~Input();

    int Parse_Input_Options(const int &argc, char *argv[]);

    int Open_Input_File(const string &file);
    int Open_Query_File(const string &file);
};
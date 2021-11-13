#pragma once

#include <iostream>
#include <vector>

#include "./LSH.h"
#include "./hyperCube.h"

using namespace std;

enum Method
{
    _Classic,
    _LSH,
    _HyperCube,
};

class kmeansplusplus
{
    private:
        int nClusters;
        bool complete;
        int lsh_k, L;           //for lsh
        int cube_k, M, probes;  //for hypercube
        int minChange = 0;
        int maxIterations = 90;
        Method method;
        Data &data;
        LSH *lsh = nullptr;
        HyperCube *cube = nullptr;
        vector<vector<uint32_t>> centroids;

        void initCentroids();
        uint32_t minDinstance_from_Centroids(const vector<uint32_t> &point);
        int findNextCentroid(const vector<double> &P, const double x);        //used for initialization
        int median(vector<uint32_t> &v);
        int minCentroid(const vector<uint32_t> &point);
        int nextCentroid(const vector<uint32_t> &point);
        void print(const vector<vector<int>> &clusters, ofstream &outputFile, int64_t time, vector<double> silouette);

        vector<vector<int>> LloydsClustering();
        vector<vector<int>> LSHClustering();
        vector<vector<int>> HyperCubeClustering();

        vector<double> Silouette(vector<vector<int>> clusters);
    
    public:
        kmeansplusplus(const int &clusters, const bool &complete, Data &data);
        kmeansplusplus(const int &clusters, const bool &complete, const int &lsh_k, const int &L, Data &data);
        kmeansplusplus(const int &clusters, const bool &complete, const int &cube_k, const int &M, const int &probes, Data &data);
        ~kmeansplusplus();

        int Run(ofstream &outputFile);
};
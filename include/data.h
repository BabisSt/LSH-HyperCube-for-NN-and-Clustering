/*
Auto to arxeio eiani gia na diabazei ta dataset kai na ta kanei parse
thelei allagh gia to pws diabazei ta dataset.
den exw katalabei pws akribws prepei na ta diabazei



*/


#pragma once

#include <fstream>
#include <vector>

using namespace std;

class Data
{
    private:
       static int EuclideanDistance(const vector<uint32_t> &p1,const vector<uint32_t> &p2);
       //static int ManhattanDistance(const vector<uint32_t> &p1, const vector<uint32_t> &p2);

    public:
        int n; // number of data
        int d; // dimension
        vector<vector<uint32_t>> data;
        vector<vector<uint32_t>> queries;

        Data();

        int InitDataSet(ifstream &inputFile);
        int ReadQueryFile(ifstream &queryFile);

        int (*distanceFunction)(const vector<uint32_t> &p1,const vector<uint32_t> &p2);

        vector<pair<int,int>>
        RangeSearch(vector<uint32_t> query, float R);

        vector<pair<int, int>>  GetClosestNeighbors(const vector<uint32_t> &query,const vector<pair<int, vector<uint32_t>>> &data, const int &N);
        vector<pair<int, int>>  BruteForceNeighbors(const vector<uint32_t> &query, const int &N);

};
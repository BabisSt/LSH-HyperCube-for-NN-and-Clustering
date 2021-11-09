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
       int EuclideanDistance(const vector<uint8_t> &p1,const vector<uint8_t> &p2);
       static int ManhattanDistance(const vector<uint8_t> &p1, const vector<uint8_t> &p2);

    public:
        int n; // number of data
        int d; // dimension
        vector<vector<uint8_t>> data;
        vector<vector<uint8_t>> queries;

        Data();

        int InitDataSet(ifstream &inputFile);
        int ReadQueryFile(ifstream &queryFile);

        int (*distanceFunction)(const vector<uint8_t> &p1,const vector<uint8_t> &p2);

        vector<pair<int,int>>
        RangeSearch(vector<uint8_t> query, float R);

        vector<pair<int, int>>  GetClosestNeighbors(const vector<uint8_t> &query,const vector<pair<int, vector<uint8_t>>> &data, const int &N);
        vector<pair<int, int>>  BruteForceNeighbors(const vector<uint8_t> &query, const int &N);

};
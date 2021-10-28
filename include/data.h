#pragma once

#include <fstream>
#include <vector>

using namespace std;

class Data
{
    private:
       static int EuclideanDistance(const vector<uint8_t> &p1,const vector<uint8_t> &p2);

    public:
        int n; // number of data
        int d; // dimension
        vector<vector<uint8_t>> data;
        vector<vector<uint8_t>> queries;

        Data();

        int InitMnistDataSet(ifstream &inputFile);
        int ReadQueryFile(ifstream &queryFile);

        int (*distanceFunction)(const vector<uint8_t> &p1,const vector<uint8_t> &p2);

        vector<pair<int,int>>
        RangeSearch(vector<uint8_t> query, float R);

        vector<pair<int, int>>  GetClosestNeighbors(const vector<uint8_t> &query,const vector<pair<int, vector<uint8_t>>> &data, const int &N);
        vector<pair<int, int>>  BruteForceNeighbors(const vector<uint8_t> &query, const int &N);

};
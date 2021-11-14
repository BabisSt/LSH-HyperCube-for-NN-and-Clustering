/*
Auto to arxeio eiani gia na diabazei ta dataset kai na ta kanei parse

*/


#pragma once

#include <fstream>
#include <vector>

using namespace std;

class Data
{
    private:
       static double EuclideanDistance(const vector<uint32_t> &p1,const vector<uint32_t> &p2);

    public:
        int n; // number of data
        int d; // dimension
        vector<vector<uint32_t>> data;
        vector<vector<uint32_t>> queries;

        Data();

        int Init_DataSet(ifstream &inputFile);       //dhmiourgia tou dataset
        int ReadQueryFile(ifstream &queryFile);     //dhmourgia tou queryset

        double (*distanceFunction)(const vector<uint32_t> &p1,const vector<uint32_t> &p2);

        vector<pair<int,int>>Range_Search(vector<uint32_t> query, float R);

        vector<pair<int, int>>  Get_Closest_Neighbors(const vector<uint32_t> &query,const vector<pair<int, vector<uint32_t>>> &data, const int &N);
        vector<pair<int, int>>  Brute_Force_Neighbors(const vector<uint32_t> &query, const int &N);

};
/*
Auto to arxeio einai gia thn dhmiourgia twn hashtable pou einai gia ta epipeda
*/

#pragma once

#include <vector>
#include <cmath>
#include <functional> //provides the standard hash function. 
#include <tuple>

using namespace std;

class hashTable
{
    private:

        int w,t,M;
    public:
        vector<vector<double>> v;

        void calculate_v(vector<vector<double>> &v, int k, int d);
        void calculate_t(int w,int k);
        
        vector<vector<tuple<uint32_t, int, reference_wrapper<vector<uint32_t>>>>> table;

        hashTable(int indexSize, int k, int d, int w,int M);
        ~hashTable();
        
        int calculate_h(vector<uint32_t> &point, vector<double> &v);

        void insertItem(uint32_t g, int index, vector<uint32_t> &point);
        vector<pair<int, reference_wrapper<vector<uint32_t>>>>
        getItems(const uint32_t &g);
};
#pragma once

#include <vector>
#include <cmath>
#include <functional>
#include <tuple>
#include <list>

using namespace std;

class hashTable
{
    private:
        int d;
        uint32_t w,M;
        vector<uint32_t> t;
    public:
        vector<vector<double>> v;

        void calculate_v(vector<vector<double>> &v, uint32_t k, uint32_t d);
        void calculate_t(uint32_t w,uint32_t k);
        
        vector<vector<tuple<uint32_t, int, reference_wrapper<vector<uint32_t>>>>> table;

        hashTable(int indexSize, uint32_t k, uint32_t d, uint32_t w,uint32_t M);
        ~hashTable();
        
        uint32_t calculate_h(const vector<uint32_t> &point,const vector<double> &v,uint32_t k);

        void Insert_Item(uint32_t g, int index, vector<uint32_t> &point);

        vector<pair<int, reference_wrapper<vector<uint32_t>>>>Get_Items(const uint32_t &g);
};
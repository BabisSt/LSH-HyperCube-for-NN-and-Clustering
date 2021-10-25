#pragma once

#include <vector>
#include <cmath>
#include <functional> //provides the standard hash function. 
#include <tuple>

using namespace std;

class hashTable
{
    private:
        int d;
        uint32_t w,m,M;         //numeric type that guarantees 32 bits
        vector<uint32_t> md;    //sequence containers representing arrays that can change in size.

        int calculate_a (const uint8_t &xi, const int &si); //&xi reference
    
    public:
        vector<vector<int>> S;
        vector<vector<tuple<uint32_t, int, reference_wrapper<vector<uint8_t>>>>> table;

        hashTable(int indexSize, int k, int d, uint32_t w, uint32_t m, uint32_t M);
        ~hashTable();

        void calculate_s(vector<vector<int>> &S, int k , int d, int W);
        uint32_t calculate_h(const vector<uint8_t> &x, const vector<int> &s);

        void insertItem(uint32_t g, int index, vector<uint8_t> &point);
        vector<pair<int, reference_wrapper<vector<uint8_t>>>>
        getItems(const uint32_t &g);
};
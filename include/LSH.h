#pragma once

#include <cmath>
#include <vector>

#include "./data.h"
#include "./hashTable.h"

using namespace std;

class LSH
{
    private:
        int k, L;
        Data &data;
        uint32_t w, M;

        int r;

        vector<hashTable *> tables;

        void hashData();

        uint32_t calculate_g(const vector<uint32_t> &points, hashTable *ht );

        void print(
                ofstream &outputFile,
                const int &query,
                vector<pair<int,int>> lshResult,
                vector<pair<int,int>> trueResult,
                const int64_t &tLSH, const int64_t &tTRUE,
                vector<pair<int,int>> rangeSearch
        ); 
    
    public:
        LSH(int , int L, Data &data, uint32_t w = 10000, int r= 5);
        ~LSH();

        int Run(const vector<vector<uint32_t>> &queries, ofstream &outputFile, const int &N, const int &R);

        vector<pair<int,int>>
        exec_query(const vector<uint32_t> &query,const int &N);
};
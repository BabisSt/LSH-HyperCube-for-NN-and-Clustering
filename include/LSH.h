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
        uint32_t w, m, M;

        vector<hashTable *> tables;

        void hashData();
        uint32_t calculate_g(const vector<uint8_t> &x, hashTable *ht);
        void print(
                ofstream &outputFile,
                const int &query,
                vector<pair<int,int>> lshResult,
                vector<pair<int,int>> trueResult,
                const int64_t &tLSH, const int64_t &tTRUE,
                vector<pair<int,int>> rangeSearch
        ); 
    
    public:
        LSH(int K, int L, Data &data, uint32_t w = 10000, uint32_t m = pow(2,30) -5 );
        ~LSH();

        int Run(const vector<vector<uint8_t>> &queries, ofstream &outputFile, const int &N, const int &R);

        vector<pair<int,int>>
        exec_query(const vector<uint8_t> &query, const int &N);
};
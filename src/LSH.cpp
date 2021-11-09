#include  <iostream>
#include <chrono>
#include <unordered_set>

#include "../include/LSH.h"
#include "../include/hashTable.h"

using namespace std;

LSH::LSH(int k, int L, Data &data, uint32_t w, uint32_t m)
    : k(k), L(L), data(data), w(w), m(m)
{
    this->M = pow(2, 32 / k);

    this->tables.resize(this->L);

    for (int i = 0; i < L; i++)
    {
        this->tables[i] = new hashTable(this->data.n / 32, this->k, this->data.d, this->w, this->m, this->M);
    }

    cout<< "Running with w: " << w << " m" << this->m << " and M:" << this->M << endl;
    
    hashData();
}

LSH::~LSH()
{
    for (int i = 0; i < L; i++)
    {
        delete this->tables[i];
    }
    
}

int LSH::Run(const vector<vector<uint8_t>> &queries, ofstream &outputFile, const int &N, const int &R)
{
    for (int i = 0; i < int(queries.size()); i++)
    {
        auto lshStart = chrono::high_resolution_clock::now();
        vector<pair<int, int>> lshResult = this->exec_query(queries[i], N);
        auto lshStop = chrono::high_resolution_clock::now();

        auto tLSH = chrono::duration_cast<chrono::milliseconds>(lshStop - lshStart);

        auto tStart = chrono::high_resolution_clock::now();
        vector<pair<int, int>> trueResult = this->data.BruteForceNeighbors(queries[i], N);
        auto tStop = chrono::high_resolution_clock::now();

        auto tTrue = chrono::duration_cast<chrono::milliseconds>(tStop - tStart);

        this->print(outputFile, i, lshResult, trueResult, tLSH.count(), tTrue.count(), this->data.RangeSearch(queries[i], R));
    }

    return 0;
    
};

void LSH::hashData()
{
    for (int i = 0; i < this->L; i++)
    {
        for (int j = 0; j < this->data.n; i++)
        {
            uint32_t g = this->calculate_g(this->data.data[j], this->tables[i]);

            this->tables[i]->insertItem(g, j, this->data.data[j]);
        }
        
    }
    
}


uint32_t LSH::calculate_g(const vector<uint8_t> &x , hashTable *ht)
{
    uint32_t g=0;

    for (int i = 0; i < this->k; i++)
    {
        g = g << 32 / this->k;
        g = g | ht->calculate_h(x, ht->S[i]);
    }
    
    return g;
}

vector<pair<int, int>> LSH::exec_query(const vector<uint8_t> &query, const int &N)
{
    unordered_set<int> pickedPoints;
    vector<pair<int, vector<uint8_t>>> possible_neighbors;

    for (auto &table : this->tables)
    {
        uint32_t g = this->calculate_g(query,table);

        for (auto &point : table->getItems(g))
        {
            if(pickedPoints.find(point.first) == pickedPoints.end()) // for duplicates
            {
                pickedPoints.insert(point.first);
                possible_neighbors.emplace_back(point.first, point.second);
            }
        }
        
    }

    return this->data.GetClosestNeighbors(query, possible_neighbors, N);

}

void LSH::print(ofstream &outputFile,const int &query,vector<pair<int,int>> lshResult,vector<pair<int,int>> trueResult,const int64_t &tLSH, const int64_t &tTRUE,vector<pair<int,int>> rangeSearch)
{
    outputFile << "Query: " << query <<  endl;
    
    for (int i = 0; i < int(lshResult.size()); i++)
    {
        outputFile << "Nearest neighbor-" << i << ": " << lshResult[i].second << endl;
        outputFile << "distanceLSH: " << lshResult[i].first << endl;
        outputFile << "distanceTrue: " << trueResult[i].first << endl;
    }
    
    outputFile << "tLSH: " << tLSH << endl;
    outputFile << "tTRUE: " << tTRUE << endl;
    outputFile << "R-near neighbors: " << endl;

    for (const auto &point : rangeSearch)
    {
        outputFile << point.second << endl;
    }
    
}

#include <list>
#include <unordered_set>
#include <vector>
#include <string>
#include <chrono>
#include <random>

#include "hashTable.h"
#include "data.h"

using namespace std;

class f
{
    private:
        unordered_set<string> setofZeros;
        unordered_set<string> setofOnes;

    public:
        f();
        ~f();

        int calculate_f(string key);
    
};

class HyperCube
{
    private:
        int k, M, probes;
        Data &data;
        hashTable *ht;
        vector<f *> fTable;

        string to_Binary(int n, int size);

        int hamming(string str1, string str2);

        list<string> Hamming_Distance(string s, int probes);

        void print(
            ofstream &outputfile,
            const int &query,
            vector<pair<int, int>> cubeResult,
            vector<pair<int, int>> trueResult,
            const int64_t &tCube, const int64_t &tTrue,
            vector<pair<int, int>> rangeSearch);
        
        void hashData();
        void HyperCube_Insert(const string &s, int index, vector<uint32_t> &point);
    
    public:
        HyperCube(int k, int M, int probes, Data &data, uint32_t w = 10000);
        ~HyperCube();

        int Run(const vector<vector<uint32_t>> &queries, ofstream &outputFileint, const int &N, const int &R);
        vector<pair<int, int>> exec_query(const vector<uint32_t> &query, const int &N);
};